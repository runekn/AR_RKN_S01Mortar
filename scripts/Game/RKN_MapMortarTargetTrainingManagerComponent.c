[ComponentEditorProps(category: "GameScripted/GameMode/Components", description: "")]
class RKN_MapMortarTargetTrainingManagerComponentClass : SCR_BaseGameModeComponentClass
{
}

class RKN_MapMortarTargetTrainingManagerComponent : SCR_BaseGameModeComponent
{
	[Attribute("{C236ADE426F4E2E5}Prefabs/Systems/ScenarioFramework/Components/RKN_MortarTargetSlot.et", UIWidgets.ResourceNamePicker, desc: "", params: "et" )]
	protected ResourceName m_sTargetSlot;

	[Attribute("{2A47D3DA85ECD809}Prefabs/Systems/ScenarioFramework/Tasks/Task.et", UIWidgets.ResourceNamePicker, desc: "", params: "et" )]
	protected ResourceName m_sTargetTask;
	
	[Attribute()]
	protected string m_sParentLayerName;
	
	[Attribute()]
	protected string m_sIdenticatorName;
	
	[Attribute("Grid %1 %2\nObserver direction %3 degrees")]
	protected LocalizedString m_sTaskDescriptionFormat;
	
	[Attribute()]
	protected string m_sActorEntityName;
	
	[Attribute(desc: "Config for the subtitles and sound event names for fire adjustments", UIWidgets.Auto)]
	ResourceName m_sAdjustSoundConfig;
	
	IEntity m_ParentEntity;
	SCR_ScenarioFrameworkLayerBase m_ParentLayer;
	IEntity m_TargetEntity;
	RKN_MortarTargetSlot m_TargetSlot;
	SCR_BaseTask m_Task;
	
	void SpawnTarget(vector pos, float targetRadius, int requiredHits, bool adjust, float adjustRadius, float adjustDirection)
	{		
		RemoveTarget();
		
		m_ParentEntity = GetGame().GetWorld().FindEntityByName(m_sParentLayerName);
		if (!m_ParentEntity)
		{
			Print("No parent layer for target slots!", LogLevel.ERROR);
			return;
		}
		m_ParentLayer = SCR_ScenarioFrameworkLayerBase.Cast(m_ParentEntity.FindComponent(SCR_ScenarioFrameworkLayerBase));
		if (!m_ParentLayer)
		{
			Print("Parent entity not a layer!", LogLevel.ERROR);
			return;
		}
		
		EntitySpawnParams params = new EntitySpawnParams();
		params.Parent = m_ParentEntity;
		params.Transform[3] = m_ParentEntity.CoordToLocal(pos);
		Resource prefab = Resource.Load(m_sTargetSlot);
		Print("Spawning at " + pos);
		m_TargetEntity = GetGame().SpawnEntityPrefab(prefab, GetGame().GetWorld(), params);
		Print("Spawned at " + m_TargetEntity.GetOrigin());
		m_TargetSlot = RKN_MortarTargetSlot.Cast(m_TargetEntity.FindComponent(RKN_MortarTargetSlot));
		
		m_TargetSlot.m_iTargetRadius = targetRadius;
		m_TargetSlot.m_iRequiredSplashesOnTarget = requiredHits;
		m_TargetSlot.m_iRequiredSplashes = 5;
		m_TargetSlot.m_bAdjustFire = adjust;
		SCR_ScenarioFrameworkGetEntityByName getter = new SCR_ScenarioFrameworkGetEntityByName();
		getter.m_sEntityName = m_sActorEntityName;
		m_TargetSlot.m_SoundActorGetter = getter;
		m_TargetSlot.m_sIndicatorEntityName = m_sIdenticatorName;
		
		if (adjust)
		{
			m_TargetSlot.m_iAdjustTargetRadius = adjustRadius;
			m_TargetSlot.m_sAdjustSoundConfig = m_sAdjustSoundConfig;
			
			m_TargetSlot.m_oObserverPosition = new PointInfo();
			vector mat[4];
			Math3D.MatrixIdentity4(mat);
			float rad = Math.DEG2RAD * adjustDirection;
			mat[3][2] = Math.Cos(rad);
			mat[3][0] = Math.Sin(rad);
			Print("Observer vector: " + mat[3]);
			m_TargetSlot.m_oObserverPosition.Set(m_ParentEntity, "", mat);
		}
		
		m_ParentLayer.RestoreToDefault(true, true, false);
		CreateTask(adjustDirection);
	}
	
	void CreateTask(float adjustDirection)
	{
		m_Task = GetTaskManager().SpawnTask(m_sTargetTask);
		m_Task.SetTitle("Mortar target");
		int gridX, gridY;
		SCR_MapEntity.GetGridPos(m_TargetEntity.GetOrigin(), gridX, gridY);
		m_Task.SetDescription(string.Format(m_sTaskDescriptionFormat, gridX.ToString(3), gridY.ToString(3), adjustDirection));
		m_Task.SetOrigin(m_TargetEntity.GetOrigin());
		m_Task.Create(true);
	}
	
	void RemoveTarget()
	{
		if (!m_TargetEntity)
			return;
		
		m_TargetSlot.Deactivate();
		SCR_EntityHelper.DeleteEntityAndChildren(m_TargetEntity);
		m_TargetEntity = null;
		m_TargetSlot = null;
		
		m_Task.Finish(true);
		
		m_ParentLayer.RestoreToDefault(true, true, false);
	}
}
