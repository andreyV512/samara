#pragma once


namespace Version
{
void SaveToFile(FILE *);
bool LoadFromFile(unsigned, FILE *);
void RestoreMemory();
//void StoreMemory();
void StoreScopeVelocity(int, double);
}

