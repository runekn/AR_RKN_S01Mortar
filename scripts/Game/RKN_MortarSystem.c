class RKN_MortarSystem : GameSystem
{
	ref array<RKN_MortarTargetSlot> m_aTargets = {};
	
	override static void InitInfo(WorldSystemInfo outInfo)
	{
		outInfo
			.SetAbstract(false)
			.AddPoint(ESystemPoint.FixedFrame);
	}
	
	static RKN_MortarSystem GetInstance()
	{
		World world = GetGame().GetWorld();
		if (!world)
			return null;

		return RKN_MortarSystem.Cast(world.FindSystem(RKN_MortarSystem));
	}
	
	void AddTarget(RKN_MortarTargetSlot target)
	{
		m_aTargets.Insert(target);
		Print("Added mortar task at " + target.GetOwner().GetOrigin());
	}
	
	void RemoveTarget(RKN_MortarTargetSlot target)
	{
		int index = m_aTargets.Find(target);
		if (index > -1)
		{
			m_aTargets.Remove(index);
			Print("Removed mortar task!");
		}
	}
	
	void Splash(vector hit, RKN_MortarHitHandlerComponent shellHandler)
	{
		// find nearest target
		RKN_MortarTargetSlot closestTarget;
		float closestDist = 0;
		for (int i = 0; i < m_aTargets.Count(); i++)
		{
			RKN_MortarTargetSlot target = m_aTargets[i];
			if (!target)
				continue;
			float dist = vector.DistanceXZ(hit, target.GetOwner().GetOrigin());
			if ((closestTarget == null || dist < closestDist) && target.IsSplash(dist, shellHandler))
			{
				closestTarget = target;
				closestDist = dist;
			}
		}
		
		if (closestTarget)
		{
			closestTarget.Splash(hit, closestDist);
		}
	}
}
