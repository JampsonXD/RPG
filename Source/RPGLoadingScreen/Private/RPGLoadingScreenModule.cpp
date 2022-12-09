#include "RPGLoadingScreenModule.h"
#include "MoviePlayer.h"
#include "Widgets/Images/SThrobber.h"

struct FRPGLoadingScreenBrush : public FSlateDynamicImageBrush, public FGCObject
{
	FRPGLoadingScreenBrush(const FName InTextureName, const FVector2D& InImageSize) : FSlateDynamicImageBrush(InTextureName, InImageSize)
	{
		SetResourceObject(LoadObject<UObject>(NULL, *InTextureName.ToString()));
	}

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override
	{
		if(UObject* CachedResourceObject = GetResourceObject())
		{
			Collector.AddReferencedObject(CachedResourceObject);
		}
	}
};

struct SRPGLoadingScreen: public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRPGLoadingScreen){}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		static const FName LoadingScreenName = TEXT("/Game/UI/Assets/LoadingScreen/LoadingScreenBrush.LoadingScreenBrush");
		LoadingScreenBrush = MakeShareable(new FRPGLoadingScreenBrush(LoadingScreenName, FVector2D(1920, 1080)));
		
		FSlateBrush *BGBrush = new FSlateBrush();
		BGBrush->TintColor = FLinearColor(0.034f, 0.034f, 0.034f, 1.0f);

		ChildSlot
			[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SBorder)	
				.BorderImage(BGBrush)
			]
			+SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SImage)
				.Image(LoadingScreenBrush.Get())
			]
			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.VAlign(VAlign_Bottom)
				.HAlign(HAlign_Right)
				.Padding(FMargin(10.0f))
				[
					SNew(SThrobber)
					.Visibility(this, &SRPGLoadingScreen::GetLoadIndicatorVisibility)
				]
			]
		];
	}


private:

	TSharedPtr<FRPGLoadingScreenBrush> LoadingScreenBrush;

	EVisibility GetLoadIndicatorVisibility() const
	{
		bool Vis = GetMoviePlayer()->IsLoadingFinished();
		return GetMoviePlayer()->IsLoadingFinished() ? EVisibility::Collapsed : EVisibility::Visible;
	}
};

class FRPGLoadingScreenModule : public IRPGLoadingScreenModule
{
	public:
	virtual ~FRPGLoadingScreenModule() = default;

	virtual void StartupModule() override
	{
		LoadObject<UObject>(NULL, TEXT("/Game/UI/Assets/LoadingScreen/LoadingScreenBrush.LoadingScreenBrush"));
		
		if(IsMoviePlayerEnabled())
		{
			CreateScreen();
		}
	}

	virtual bool IsGameModule() const override
	{
		return true;
	}

	virtual void StartInGameLoadingScreen(bool bPlayUntilStopped, float Playtime) override
	{
		FLoadingScreenAttributes LoadingScreenAttributes;
		LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = bPlayUntilStopped;
		LoadingScreenAttributes.bWaitForManualStop = bPlayUntilStopped;
		LoadingScreenAttributes.bAllowEngineTick = bPlayUntilStopped;
		LoadingScreenAttributes.MinimumLoadingScreenDisplayTime = Playtime;
		LoadingScreenAttributes.WidgetLoadingScreen = SNew(SRPGLoadingScreen);
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes);
		GetMoviePlayer()->PlayMovie();
	}

	virtual void StopInGameLoadingScreen() override
	{
		GetMoviePlayer()->StopMovie();
	}

	virtual void CreateScreen()
	{
		FLoadingScreenAttributes LoadingScreenAttributes;
		LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = true;
		LoadingScreenAttributes.MinimumLoadingScreenDisplayTime = 5.f;
		LoadingScreenAttributes.WidgetLoadingScreen = SNew(SRPGLoadingScreen);
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes);
	}
};

IMPLEMENT_MODULE(FRPGLoadingScreenModule, RPGLoadingScreen);
