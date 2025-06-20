[BaseContainerProps(), SCR_ContainerActionTitle()]
class RKN_PlayerSitAction : SCR_ScenarioFrameworkActionBase
{
	ImageWidget m_wFadeOut;
	WidgetAnimationOpacity m_OngoingFadeAnimation;
	
	protected const string FADEOUTLAYOUT = "{265245C299401BF6}UI/layouts/Menus/ContentBrowser/DownloadManager/ScrollBackground.layout";
	
	override void OnActivate(IEntity object)
	{
		Fadeout(true);
		IEntity player = GetGame().GetPlayerController().GetControlledEntity();
		SCR_CharacterControllerComponent characterController = SCR_CharacterControllerComponent.Cast(player.FindComponent(SCR_CharacterControllerComponent));
		if (!characterController)
			return;
		
		IEntity position = object;
		if (!position)
			return;
		
		vector bedTransformation[4];
		position.GetTransform(bedTransformation);
		
		characterController.StartLoitering(8, true, true, true, bedTransformation, false);
		
 		GetGame().GetCallqueue().CallLater(Fadeout, 1500, false, false, 1, 0);
	}
	
	void Fadeout(bool fadeOut, float duration = 1, float soundDuration = duration)
	{
		if (!m_wFadeOut)
		{
			SCR_HUDManagerComponent hudManager = GetGame().GetHUDManager();
			if (!hudManager)
				return;
			
			m_wFadeOut = ImageWidget.Cast(hudManager.CreateLayout(FADEOUTLAYOUT, EHudLayers.OVERLAY));
			
			if (!m_wFadeOut)
				return;
		}
		
		if (fadeOut)
		{
			m_wFadeOut.SetOpacity(1);
		}
		else
		{
			if (!m_OngoingFadeAnimation)
			{
				m_OngoingFadeAnimation = AnimateWidget.Opacity(m_wFadeOut, fadeOut, duration);
			}
			else
			{
				m_OngoingFadeAnimation.SetTargetValue(fadeOut);
				m_OngoingFadeAnimation.SetSpeed(duration);
			}
		}
		
	}
}
