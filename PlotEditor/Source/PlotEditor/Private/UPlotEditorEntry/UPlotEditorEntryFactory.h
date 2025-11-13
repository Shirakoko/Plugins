#pragma once

#include "UPlotEditorEntryFactory.generated.h"

UCLASS()
class UPlotEditorEntryFactory : public UFactory
{
	GENERATED_BODY()
public:
	UPlotEditorEntryFactory();

	//~ Begin UFactory Interface
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	//~ Begin UFactory Interface
};