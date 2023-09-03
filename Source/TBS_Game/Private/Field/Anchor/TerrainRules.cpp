#include "Field/Anchor/TerrainRules.h"

FSingleTerrainRule FTerrainRules::GetTerrainRule(const ETerrainType TerrainType) const
{
	if (const FSingleTerrainRule* Rule = Rules.FindByPredicate([&TerrainType](const FSingleTerrainRule& Rule)
	{
		return Rule.TerrainType == TerrainType;
	}))
	{
		return *Rule;
	}
	return FTerrainRules().GetTerrainRule(TerrainType);
	// IMPORTANT: this can go into a recursion if the default constructor of FTerrainRules does not contains all terrain types
}
