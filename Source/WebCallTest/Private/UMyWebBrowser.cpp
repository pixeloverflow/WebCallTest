// Fill out your copyright notice in the Description page of Project Settings.


#include "UMyWebBrowser.h"

#include "crypto_aes256cbc.h"
#include "crypto_base64.h"
#include "Runtime/WebBrowser/Public/SWebBrowser.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Async/TaskGraphInterfaces.h"
#include "UObject/ConstructorHelpers.h"

#if WITH_EDITOR
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialExpressionMaterialFunctionCall.h"
#include "Materials/MaterialExpressionTextureSample.h"
#include "Materials/MaterialExpressionTextureSampleParameter2D.h"
#include "Materials/MaterialFunction.h"
#include "Factories/MaterialFactoryNew.h"
#include "AssetRegistryModule.h"
#include "PackageHelperFunctions.h"
#endif

#define LOCTEXT_NAMESPACE "WebBrowser"

/////////////////////////////////////////////////////
// UWebBrowser

UMyWebBrowser::UMyWebBrowser(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsVariable = true;
}

void UMyWebBrowser::BindUObject(const FString & Name, UObject * Object, bool bIsPermanent)
{
	if (WebBrowserWidget.IsValid())
	{
		WebBrowserWidget->BindUObject(Name, Object, bIsPermanent);
	}
}

void UMyWebBrowser::LoadURL(FString NewURL)
{
	if (WebBrowserWidget.IsValid())
	{
		return WebBrowserWidget->LoadURL(NewURL);
	}
	
	// unsigned char iv[16] = {
	// 	0x31, 0x61, 0x48, 0x6a, 0x61, 0x38, 0x6c, 0x50, 0x4d, 0x51, 0x79, 0x56, 0x71, 0x49, 0x42, 0x2d
	// };
	// unsigned char key[32] = {
	// 	0x31, 0x61, 0x48, 0x6a, 0x61, 0x38, 0x6c, 0x50, 0x4d, 0x51, 0x79, 0x56, 0x71, 0x49, 0x42, 0x2d, 0x78, 0x42,
	// 	0x58, 0x75, 0x4e, 0x37, 0x42, 0x45, 0x43, 0x4d, 0x49, 0x51, 0x49, 0x7a, 0x34, 0x4d
	// }; // 256 bit

	// FString key = TEXT( "1aHja8lPMQyVqIB-xBXuN7BECMIQIz4M");
	// FString iv = TEXT("1aHja8lPMQyVqIB-" );
	unsigned char hex_dump[128];
	CRYPTO_AES256CBC_CTX ctx_aes256cbc;

	unsigned char source[] = "m2V7fIjol4qvqgqJN5U5Ola0OguI/a0zLYi+h7/apAlSNBwbJUX5vcd9FAAFTRc5beooXek+XIUyYNt/2l7eEg==";
	size_t source_len = strlen((char*)source);
	unsigned char result_base64[128];
	size_t result_len;
	CRYPTO_BASE64_Decode(result_base64, &result_len, source, source_len);
	result_base64[result_len] = '\0';
	unsigned char iv[] = "1aHja8lPMQyVqIB-";
	unsigned char key[] = "1aHja8lPMQyVqIB-xBXuN7BECMIQIz4M";
	
	CRYPTO_AES256CBC_init_ctx_iv(&ctx_aes256cbc, key, iv);
	CRYPTO_AES256CBC_decrypt(&ctx_aes256cbc, result_base64, result_len);
    
	memset(hex_dump, 0, sizeof(hex_dump));
	memcpy(hex_dump, result_base64, result_len);
	CRYPTO_AES256CBC_hex_dump(hex_dump, result_base64, result_len);
	// unsigned char to fstring
	FString Fs = FString(UTF8_TO_TCHAR(hex_dump));
	UE_LOG(LogTemp, Display, TEXT("Matinee preview default pawn class is missing. %s"), *Fs);

	char new_array[sizeof(result_base64)];
	int j = 0;

	for (int i = 0; result_base64[i] != '\0'; i++) {
		if (result_base64[i] != '\x1C') {
			new_array[j++] = result_base64[i];
		}
	}
	new_array[j] = '\0';
	FString Fs2 = FString(UTF8_TO_TCHAR(new_array));
	UE_LOG(LogTemp, Display, TEXT("Matinee preview default pawn class is missing. %s"), *Fs2);
}

void UMyWebBrowser::LoadString(FString Contents, FString DummyURL)
{
	if (WebBrowserWidget.IsValid())
	{
		return WebBrowserWidget->LoadString(Contents, DummyURL);
	}
}

void UMyWebBrowser::ExecuteJavascript(const FString& ScriptText)
{
	if (WebBrowserWidget.IsValid())
	{
		return WebBrowserWidget->ExecuteJavascript(ScriptText);
	}
}

FText UMyWebBrowser::GetTitleText() const
{
	if (WebBrowserWidget.IsValid())
	{
		return WebBrowserWidget->GetTitleText();
	}

	return FText::GetEmpty();
}

FString UMyWebBrowser::GetUrl() const
{
	if (WebBrowserWidget.IsValid())
	{
		return WebBrowserWidget->GetUrl();
	}

	return FString();
}

void UMyWebBrowser::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	WebBrowserWidget.Reset();
}

TSharedRef<SWidget> UMyWebBrowser::RebuildWidget()
{
	if (IsDesignTime())
	{
		return SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("Web Browser", "Web Browser"))
			];
	}
	else
	{
		WebBrowserWidget = SNew(SWebBrowser)
			.InitialURL(InitialURL)
			.ShowControls(false)
			.SupportsTransparency(bSupportsTransparency)
			.OnUrlChanged(BIND_UOBJECT_DELEGATE(FOnTextChanged, HandleOnUrlChanged))
			.OnBeforePopup(BIND_UOBJECT_DELEGATE(FOnBeforePopupDelegate, HandleOnBeforePopup));

		return WebBrowserWidget.ToSharedRef();
	}
}

void UMyWebBrowser::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (WebBrowserWidget.IsValid())
	{

	}
}

void UMyWebBrowser::HandleOnUrlChanged(const FText& InText)
{
	OnUrlChanged.Broadcast(InText);
}

bool UMyWebBrowser::HandleOnBeforePopup(FString URL, FString Frame)
{
	if (OnBeforePopup.IsBound())
	{
		if (IsInGameThread())
		{
			OnBeforePopup.Broadcast(URL, Frame);
		}
		else
		{
			// Retry on the GameThread.
			TWeakObjectPtr<UMyWebBrowser> WeakThis = this;
			FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis, URL, Frame]()
			{
				if (WeakThis.IsValid())
				{
					WeakThis->HandleOnBeforePopup(URL, Frame);
				}
			}, TStatId(), nullptr, ENamedThreads::GameThread);
		}

		return true;
	}

	return false;
}

#if WITH_EDITOR

const FText UMyWebBrowser::GetPaletteCategory()
{
	return LOCTEXT("My", "My");
}

#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE