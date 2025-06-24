[ComponentEditorProps(category: "GameScripted/Misc", description: "")]
class RKN_MortarTargetIndicatorComponentClass : ScriptComponentClass
{
}


class RKN_MortarTargetIndicatorComponent : ScriptComponent
{
	[Attribute("{F2E83D562703F861}Assets/Decals/Impact/DecalHitIndicator.emat", UIWidgets.ResourceNamePicker, "Material used for target hit indicator", "emat")]
	private ResourceName m_HitDecal;
	[Attribute("{F2E83D562703F861}Assets/Decals/Impact/DecalHitIndicator.emat", UIWidgets.ResourceNamePicker, "Material used for target hit indicator", "emat")]
	private ResourceName m_RadiusDecal;
	[Attribute(defvalue: "200")]
	private float m_fScale;
	
	ref array<vector> m_aHits = {};
	ref array<Decal> m_aDecals = {};
	
	void RecordHit(vector hit)
	{
		m_aHits.Insert(hit);
	}
	
	void ShowIndicators(vector target, float radius)
	{
		World world = GetGame().GetWorld();
		foreach (Decal decal : m_aDecals)
		{
			world.RemoveDecal(decal);
		}
		
		
		Decal targetDecal = ShowTargetIndicator(radius);
		m_aDecals.Insert(targetDecal);
		foreach (vector hit : m_aHits)
		{
			Decal decal = ShowIndicator(hit, target);
			m_aDecals.Insert(decal);
		}
	}
	
	private Decal ShowIndicator(vector hit, vector target)
	{
		vector direction = vector.Direction(target, hit);
		vector scaledDirection = { direction[0] / m_fScale, (direction[2] / m_fScale) + 0.5, 0.02 };
		Print("placing dot: " + scaledDirection);
		
		World world = GetGame().GetWorld();
		IEntity owner = GetOwner();
		return world.CreateDecal(
				owner, // Entity 
				owner.CoordToParent(scaledDirection), // origin vector (position) 
				owner.VectorToParent({ 0, 0, -1 }), // project vector 
				0.0, // nearclip
				0.2, // farclip
				0, // angle 
				0.075, // size 
				1, // stretch 
				m_HitDecal, //emat path
				-1,// lifetime, if <= 0 the decal is created as static
				0xFFFFFFFF); //color of decal
	}
	
	private Decal ShowTargetIndicator(float radius)
	{
		World world = GetGame().GetWorld();
		IEntity owner = GetOwner();
		return world.CreateDecal(
				owner, // Entity 
				owner.CoordToParent({ 0, 0.5, 0 }), // origin vector (position) 
				owner.VectorToParent({ 0, 0, -1 }), // project vector 
				0.0, // nearclip
				0.2, // farclip
				0, // angle 
				radius / (m_fScale / 2), // size 
				1, // stretch 
				m_RadiusDecal, //emat path
				-1,// lifetime, if <= 0 the decal is created as static
				0xFFFFFFFF); //color of decal
	}
}
