[ComponentEditorProps(category: "GameScripted/Misc", description: "")]
class RKN_MortarHitHandlerComponentClass : ScriptComponentClass
{
}

class RKN_MortarHitHandlerComponent : ScriptComponent
{
	[Attribute(typename.EnumToString(SCR_EAIArtilleryAmmoType, SCR_EAIArtilleryAmmoType.HIGH_EXPLOSIVE), UIWidgets.ComboBox, desc: "Ammo type", enumType: SCR_EAIArtilleryAmmoType)]
	SCR_EAIArtilleryAmmoType m_eAmmoType;
	
	vector m_vOrigin;
	
	//------------------------------------------------------------------------------------------------
	override void OnDelete(IEntity owner)
	{
		if (!Replication.IsServer())
			return;
		
		Print("Splash: " + owner.GetOrigin());
		
		RKN_MortarSystem system = RKN_MortarSystem.GetInstance();
		if (system)
			system.Splash(owner.GetOrigin(), this);
	}
	
	void ShellFired(BaseMuzzleComponent muzzle)
	{
		Print("Fired shell " + GetOwner().GetName());
		m_vOrigin = muzzle.GetOwner().GetOrigin();
	}
}
