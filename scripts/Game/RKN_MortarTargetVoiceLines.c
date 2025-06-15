[BaseContainerProps(configRoot: true)]
class RKN_MortarTargetVoiceLines
{
	
	[Attribute(defvalue: "SOUND_FIREMISSION_ADJUST_PREFIX", category: "Prefix")]
	string m_sAdjustPrefixSoundEvent;
	
	[Attribute("0", category: "Prefix")]
	int m_iAdjustPrefixIndex;
	
	[Attribute(category: "Prefix")]
	string m_sAdjustPrefixSubtitles;
	
	[Attribute("SOUND_FIREMISSION_ADJUST_SUFFIX", category: "Suffix")]
	string m_sAdjustSuffixSoundEvent;
	
	[Attribute("0", category: "Suffix")]
	int m_iAdjustSuffixIndex;
	
	[Attribute(category: "Suffix")]
	string m_sAdjustSuffixSubtitles;
	
	[Attribute("SOUND_FIREMISSION_ADJUST_HIT", category: "Hit")]
	string m_sAdjustHitSoundEvent;
	
	[Attribute("0", category: "Hit")]
	int m_iAdjustHitIndex;
	
	[Attribute(category: "Hit")]
	string m_sAdjustHitSubtitles;
	
	[Attribute("SOUND_FIREMISSION_ADJUST", category: "Adjustment")]
	string m_sAdjustSoundEvent;
	
	[Attribute(defvalue: "1", params: "0 inf", category: "Adjustment")]
	int m_iOverIndex;
	
	[Attribute(defvalue: "Drop", category: "Adjustment")]
	string m_sOverSubtitles;
	
	[Attribute(defvalue: "2", params: "0 inf", category: "Adjustment")]
	int m_iShortIndex;
	
	[Attribute(defvalue: "Add", category: "Adjustment")]
	string m_sShortSubtitles;
	
	[Attribute(defvalue: "1", params: "0 inf", category: "Adjustment")]
	int m_iLeftIndex;
	
	[Attribute(defvalue: "Right", category: "Adjustment")] // opposite since they are directions for next shot
	string m_sLeftSubtitles;
	
	[Attribute(defvalue: "2", params: "0 inf", category: "Adjustment")]
	int m_iRightIndex;
	
	[Attribute(defvalue: "Left", category: "Adjustment")] // opposite since they are directions for next shot
	string m_sRightSubtitles;
	
	[Attribute(desc: "Must be in ascending order", category: "Adjustment")]
	ref array<ref RKN_MortarTargetDistanceConfig> m_aDistances;
}

[BaseContainerProps(configRoot: false)]
class RKN_MortarTargetDistanceConfig
{
	[Attribute(params: "0 inf")]
	int m_iDistance;
	[Attribute()]
	int m_iIndex;
	[Attribute(desc: "Leave empty to use distance")]
	string m_sSubtitles;
}