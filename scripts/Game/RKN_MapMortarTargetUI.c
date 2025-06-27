class RKN_MapMortarTargetUI : SCR_MapUIBaseComponent
{
	[Attribute("Mortar target", UIWidgets.Auto, "Menu category name" )]
	protected string m_sCategoryName;
	
	[Attribute("{3262679C50EF4F01}UI/Textures/Icons/icons_wrapperUI.imageset", UIWidgets.ResourceNamePicker, desc: "Icons imageset", params: "imageset" )]
	protected ResourceName m_sIconImageset;
	
	[Attribute("artilleryFireMission", UIWidgets.Auto, "Category icon quad" )]
	protected string m_sCategoryIconName;
	
	[Attribute("{3262679C50EF4F01}UI/Textures/Icons/icons_wrapperUI.imageset", UIWidgets.ResourceNamePicker, desc: "Icons imageset", params: "imageset" )]
	protected ResourceName m_sDeleteIconImageset;
	
	[Attribute("cancel", UIWidgets.Auto, "Category icon quad" )]
	protected string m_sDeleteIconName;
	
	[Attribute("{ECEC2704C83C8779}UI/layouts/Map/MapMortarTargetEditBox.layout", UIWidgets.ResourceNamePicker, desc: "Edit box dialog when placing a target", params: "layout" )]
	protected ResourceName m_sEditBoxLayout;
	
	protected RKN_MapMortarTargetTrainingManagerComponent m_Manager;
	protected SCR_MapCursorModule m_CursorModule;
	protected SCR_SelectionMenuCategoryEntry m_RootCategoryEntry;
	protected Widget m_EditRoot;
	
	SCR_SliderComponent m_TargetRadiusSliderComp;
	SCR_SliderComponent m_RequiredHitsSliderComp;
	SCR_CheckboxComponent m_AdjustCheckboxComp;
	SCR_SliderComponent m_AdjustRadiusSliderComp;
	SCR_SliderComponent m_AdjustObserverSliderComp;
	Widget m_AdjustRadiusSlider;
	Widget m_AdjustObserverSlider;
	
	
	override void Init()
	{
		m_Manager = RKN_MapMortarTargetTrainingManagerComponent.Cast(GetGame().GetGameMode().FindComponent(RKN_MapMortarTargetTrainingManagerComponent));
		if (!m_Manager)
			return;
		
		SCR_MapRadialUI radialMenu = SCR_MapRadialUI.Cast(m_MapEntity.GetMapUIComponent(SCR_MapRadialUI));
		if (radialMenu)
		{
			radialMenu.GetOnMenuInitInvoker().Insert(OnRadialMenuInit);
			//radialMenu.GetRadialController().GetOnInputOpen().Insert(OnRadialMenuOpen);
		}
		
		m_CursorModule = SCR_MapCursorModule.Cast(m_MapEntity.GetMapModule(SCR_MapCursorModule));
	}
	
	protected void OnRadialMenuInit()
	{		
		SCR_MapRadialUI radialUI = SCR_MapRadialUI.Cast(m_MapEntity.GetMapUIComponent(SCR_MapRadialUI));

		m_RootCategoryEntry = radialUI.AddRadialCategory(m_sCategoryName);
		m_RootCategoryEntry.SetIcon(m_sIconImageset, m_sCategoryIconName);
		
		SCR_SelectionMenuEntry menuEntry = new SCR_SelectionMenuEntry();
		menuEntry.SetName("Place mortar target");
		menuEntry.GetOnPerform().Insert(SpawnEditModal);
		menuEntry.SetIcon(m_sIconImageset, m_sCategoryIconName);
		
		radialUI.InsertCustomRadialEntry(menuEntry, m_RootCategoryEntry);	
		
		menuEntry = new SCR_SelectionMenuEntry();
		menuEntry.SetName("Delete target");
		menuEntry.GetOnPerform().Insert(DeleteTarget);
		menuEntry.SetIcon(m_sDeleteIconImageset, m_sDeleteIconName);
		
		radialUI.InsertCustomRadialEntry(menuEntry, m_RootCategoryEntry);	
	}
	
	protected void DeleteTarget(SCR_SelectionMenuEntry entry)
	{
		m_Manager.RemoveTarget();
	}
	
	protected void SpawnEditModal(SCR_SelectionMenuEntry entry)
	{
		if (m_EditRoot)
			CleanupEditWidget();
		
		m_EditRoot = GetGame().GetWorkspace().CreateWidgets(m_sEditBoxLayout, m_RootWidget);
		
		float screenX, screenY;
		m_MapEntity.GetMapWidget().GetScreenSize(screenX, screenY);
		FrameSlot.SetPos(m_EditRoot, GetGame().GetWorkspace().DPIUnscale(screenX * 0.5), GetGame().GetWorkspace().DPIUnscale(screenY * 0.5));
		
		Widget slider = m_EditRoot.FindAnyWidget("RadiusSliderRoot");
		m_TargetRadiusSliderComp = SCR_SliderComponent.Cast(slider.FindHandler(SCR_SliderComponent));
		
		slider = m_EditRoot.FindAnyWidget("RequiredHitsSliderRoot");
		m_RequiredHitsSliderComp = SCR_SliderComponent.Cast(slider.FindHandler(SCR_SliderComponent));
		
		m_AdjustRadiusSlider = m_EditRoot.FindAnyWidget("AdjustRadiusSliderRoot");
		m_AdjustRadiusSliderComp = SCR_SliderComponent.Cast(m_AdjustRadiusSlider.FindHandler(SCR_SliderComponent));
		
		m_AdjustObserverSlider = m_EditRoot.FindAnyWidget("ObserverSliderRoot");
		m_AdjustObserverSliderComp = SCR_SliderComponent.Cast(m_AdjustObserverSlider.FindHandler(SCR_SliderComponent));
		
		Widget checkbox = m_EditRoot.FindAnyWidget("AdjustCheckbox");
		m_AdjustCheckboxComp = SCR_CheckboxComponent.Cast(checkbox.FindHandler(SCR_CheckboxComponent));
		m_AdjustCheckboxComp.m_OnChanged.Insert(OnAdjustChanged);
		
		SCR_InputButtonComponent confirmComp = SCR_InputButtonComponent.Cast(m_EditRoot.FindAnyWidget("ButtonConfirm").FindHandler(SCR_InputButtonComponent));
		confirmComp.m_OnClicked.Insert(OnEditConfirm);
		
		SCR_InputButtonComponent cancelComp = SCR_InputButtonComponent.Cast(m_EditRoot.FindAnyWidget("ButtonCancel").FindHandler(SCR_InputButtonComponent));
		cancelComp.m_OnClicked.Insert(OnEditCancelled);
		
		m_CursorModule.HandleDialog(true);
	}
	
	protected void OnAdjustChanged(SCR_SliderComponent slider, bool value)
	{
		m_AdjustObserverSlider.SetEnabled(value);
		m_AdjustRadiusSlider.SetEnabled(value);
	}
	
	protected void OnEditConfirm(SCR_InputButtonComponent button)
	{
		float wX, wZ;
		m_MapEntity.GetMapCenterWorldPosition(wX, wZ);
		vector pos = { wX, 0, wZ };
		pos[1] = SCR_TerrainHelper.GetTerrainY(pos, GetGame().GetWorld());
		string grid = SCR_MapEntity.GetGridLabel(pos);
		Print("Placing mortar target at pos=" + pos + " grid: " + grid);
		m_Manager.SpawnTarget(pos, m_TargetRadiusSliderComp.GetValue(), m_RequiredHitsSliderComp.GetValue(), m_AdjustCheckboxComp.IsChecked(), m_AdjustRadiusSliderComp.GetValue(), m_AdjustObserverSliderComp.GetValue());
		
		CleanupEditWidget();
	}
	
	protected void OnEditCancelled(SCR_InputButtonComponent button)
	{
		CleanupEditWidget();
	}
	
	protected void CleanupEditWidget()
	{
		if (m_EditRoot)
			m_EditRoot.RemoveFromHierarchy();
		
		//m_bIsDelayed = false;
				
		m_CursorModule.HandleDialog(false);
	}
}
