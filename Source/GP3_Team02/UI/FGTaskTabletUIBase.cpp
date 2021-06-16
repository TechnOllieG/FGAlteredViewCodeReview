#include "FGTaskTabletUIBase.h"

TArray<AFGTaskHandler*> UFGTaskTabletUIBase::SortTaskHandlers(TArray<AFGTaskHandler*> TaskHandlers)
{
	int MaxValue;
	int MaxIndex;
	
	for(int i = 0; i < TaskHandlers.Num() - 1; i++)
	{
		MaxValue = TaskHandlers[i]->TaskPriority;
		MaxIndex = i;
		
		for(int j = i + 1; j < TaskHandlers.Num(); j++)
		{
			if(TaskHandlers[j]->TaskPriority > MaxValue)
			{
				MaxValue = TaskHandlers[j]->TaskPriority;
				MaxIndex = j;
			}
		}
		
		if(i != MaxIndex)
			TaskHandlers.Swap(MaxIndex, i);
	}

	return TaskHandlers;
}
