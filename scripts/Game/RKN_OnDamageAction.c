[BaseContainerProps(), SCR_ContainerActionTitle()]
class RKN_OnDamageAction : SCR_ScenarioFrameworkActionBase
{
	[Attribute(desc: "Entity to check.")]
	ref SCR_ScenarioFrameworkGet m_Getter;
	
	[Attribute()]
	protected ref array<ref RKN_DamageCondition> m_aDamageConditions;
	
	[Attribute()]
	protected ref array<ref SCR_ScenarioFrameworkActivationConditionBase> m_aOtherConditions;

	[Attribute(defvalue: SCR_EScenarioFrameworkLogicOperators.AND.ToString(), UIWidgets.ComboBox, "Which Boolean Logic will be used for Activation Conditions", "", enums: SCR_EScenarioFrameworkLogicOperatorHelper.GetParamInfo())]
	SCR_EScenarioFrameworkLogicOperators m_eActivationConditionLogic;

	[Attribute(desc: "Actions to be executed if conditions' evaluation is successful.")]
	ref array<ref SCR_ScenarioFrameworkActionBase> m_aActions;
	
	[Attribute(desc: "Actions to be executed if conditions' evaluation is failed.")] 
	ref array<ref SCR_ScenarioFrameworkActionBase> m_aFailedActions;
	
	[Attribute(defvalue: "true")]
	bool m_bRemoveOnSuccess;
	
	IEntity m_Object;
	ref array<SCR_DamageManagerComponent> m_aDamageManagers = {};
	
	override void OnActivate(IEntity object)
	{
		m_Object = object;
		if (m_Getter)
		{
			SCR_ScenarioFrameworkParam<IEntity> entityWrapper = SCR_ScenarioFrameworkParam<IEntity>.Cast(m_Getter.Get());
			m_Object = entityWrapper.GetValue();
		}
		
		IEntity damagableObject = m_Object;
		SCR_ScenarioFrameworkLayerBase layer = SCR_ScenarioFrameworkLayerBase.Cast(m_Object.FindComponent(SCR_ScenarioFrameworkLayerBase));
		if (layer)
			damagableObject = layer.GetSpawnedEntity();
		
		if (!damagableObject)
		{
			Print("No entity found!", LogLevel.ERROR);
			return;
		}
		
		SCR_AIGroup group = SCR_AIGroup.Cast(damagableObject);
		if (group)
		{
			array<AIAgent> agents = {};
			group.GetAgents(agents);
			foreach (AIAgent agent : agents)
				AddDamageCallback(agent.GetControlledEntity());
		}
		else
		{
			AddDamageCallback(damagableObject);
		}
	}
	
	void AddDamageCallback(IEntity object)
	{
		SCR_DamageManagerComponent damageManager = SCR_DamageManagerComponent.Cast(object.FindComponent(SCR_DamageManagerComponent));
		if (!damageManager)
		{
			Print("Entity has no DamageManager: " + object, LogLevel.ERROR);
			return;
		}
		
		damageManager.GetOnDamage().Insert(CheckDamage);
		m_aDamageManagers.Insert(damageManager);
	}
	
	void CheckDamage(notnull BaseDamageContext damageContext)
	{
		array<ref SCR_ScenarioFrameworkActivationConditionBase> conditions = {};
		foreach (SCR_ScenarioFrameworkActivationConditionBase condition : m_aOtherConditions)
			conditions.Insert(condition);
		foreach (RKN_DamageCondition condition : m_aDamageConditions)
			conditions.Insert(new RKN_DamageConditionWrapper(damageContext, condition));
		
		if (SCR_ScenarioFrameworkActivationConditionBase.EvaluateEmptyOrConditions(m_eActivationConditionLogic, conditions, m_Object))
		{
			foreach (SCR_ScenarioFrameworkActionBase actions : m_aActions)
			{
				if (m_bRemoveOnSuccess)
					foreach (SCR_DamageManagerComponent damageManager : m_aDamageManagers)
						damageManager.GetOnDamage().Remove(CheckDamage);
				
				actions.OnActivate(m_Object);
			}
		}
		else
		{
			foreach (SCR_ScenarioFrameworkActionBase actions : m_aFailedActions)
			{
				actions.OnActivate(m_Object);
			}
		}
	}
}

[BaseContainerProps()]
class RKN_DamageCondition
{
	bool Init(IEntity entity, notnull BaseDamageContext damageContext);
}

class RKN_DamageConditionWrapper : SCR_ScenarioFrameworkActivationConditionBase
{
	BaseDamageContext m_DamageContext;
	RKN_DamageCondition m_DamageCondition;
	
	void RKN_DamageConditionWrapper(BaseDamageContext damageContext, RKN_DamageCondition condition)
	{
		this.m_DamageContext = damageContext;
		this.m_DamageCondition = condition;
	}
	
	override bool Init(IEntity entity)
	{
		return m_DamageCondition.Init(entity, m_DamageContext);
	}
}
