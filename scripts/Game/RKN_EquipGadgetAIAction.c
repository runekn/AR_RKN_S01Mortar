// Code stolen from SCR_AITakeGadgetInLeftHand

[BaseContainerProps(), SCR_ContainerAIActionTitle()]
class RKN_EquipGadgetAIAction : SCR_ScenarioFrameworkAIAction
{
	[Attribute("0", UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(EGadgetType) )]
	private EGadgetType m_eGadgetType;
	
	override void OnActivate()
	{
		super.OnActivate();
		array<AIAgent> agents = {};
		m_AIGroup.GetAgents(agents);
		foreach (AIAgent agent : agents)
		{
			IEntity character = agent.GetControlledEntity();
			SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(character.FindComponent(SCR_InventoryStorageManagerComponent));
			CharacterControllerComponent characterController = CharacterControllerComponent.Cast(character.FindComponent(CharacterControllerComponent));
			SCR_GadgetManagerComponent gadgetManager = SCR_GadgetManagerComponent.GetGadgetManager(character);
			if (!inventoryManager || !characterController || !gadgetManager)
				return;
			
			IEntity item = gadgetManager.GetGadgetByType(m_eGadgetType);
			if (!item)
			{
				Print("Item of type " + m_eGadgetType + " not found!", LogLevel.WARNING);
				return;
			}
			if (IsItemEquipped(characterController, item))
			{
				if (m_bDebug)
					Print("Item already equipped");
				return;
			}
			if (m_bDebug)
				Print("Equipping " + item.GetName());
			gadgetManager.SetGadgetMode(item, EGadgetMode.IN_HAND);
			characterController.SetGadgetRaisedModeWanted(true);
		}
	}
	
	//-----------------------------------------------------------------------------------------------
	private bool IsItemEquipped(CharacterControllerComponent characterController, IEntity item)
	{
		if (!item)
			return false;
		
		IEntity itemAtSlot = characterController.GetAttachedGadgetAtLeftHandSlot();
		
		return itemAtSlot == item;
	}
	
}
