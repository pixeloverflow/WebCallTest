// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "UMyWebBrowser.generated.h"

UCLASS()
class WEBCALLTEST_API UMyWebBrowser : public UWidget
{
	GENERATED_UCLASS_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUrlChanged, const FText&, Text);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBeforePopup, FString, URL, FString, Frame);

	UFUNCTION(BlueprintCallable, Category = "Web Browser")
	void BindUObject(const FString& Name, UObject* Object, bool bIsPermanent);

	UFUNCTION(BlueprintCallable, Category = "Web Browser")
	void LoadURL(FString NewURL);

	UFUNCTION(BlueprintCallable, Category = "Web Browser")
	void LoadString(FString Contents, FString DummyURL);


	UFUNCTION(BlueprintCallable, Category = "Web Browser")
	void ExecuteJavascript(const FString& ScriptText);


	UFUNCTION(BlueprintCallable, Category = "Web Browser")
	FText GetTitleText() const;


	UFUNCTION(BlueprintCallable, Category = "Web Browser")
	FString GetUrl() const;


	UPROPERTY(BlueprintAssignable, Category = "Web Browser|Event")
	FOnUrlChanged OnUrlChanged;

	UPROPERTY(BlueprintAssignable, Category = "Web Browser|Event")
	FOnBeforePopup OnBeforePopup;

public:
	virtual void SynchronizeProperties() override;

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

protected:
	UPROPERTY(EditAnywhere, Category = Appearance)
	FString InitialURL;

	UPROPERTY(EditAnywhere, Category = Appearance)
	bool bSupportsTransparency;

protected:
	TSharedPtr<class SWebBrowser> WebBrowserWidget;

protected:
	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget interface

	void HandleOnUrlChanged(const FText& Text);
	bool HandleOnBeforePopup(FString URL, FString Frame);
};
