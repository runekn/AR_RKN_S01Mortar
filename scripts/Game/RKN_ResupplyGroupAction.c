[BaseContainerProps(), SCR_ContainerAIActionTitle()]
class RKN_ResupplyGroupAction : SCR_ScenarioFrameworkAIAction
{
	[Attribute(defvalue: "6", desc: "Amount of magazines the action will ensure are in the inventory (fill to this amount)")]
	private int m_MagazineQuantityInInventory;
	
	override void OnActivate()
	{
		super.OnActivate();
		foreach (SCR_ChimeraCharacter character : m_AIGroup.GetAIMembers())
		{
			SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(character.FindComponent(SCR_InventoryStorageManagerComponent));
			if (!inventoryManager)
				return;
			
			inventoryManager.ResupplyMagazines(m_MagazineQuantityInInventory);
		}
	}
}
