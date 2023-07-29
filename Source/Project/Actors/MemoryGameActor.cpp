// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MemoryGameActor.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

#define END_DURATION 5.0f
#define GLOW_INTENSITY 10.0f
#define CONTINUE_INITIAL_GAP 2.0f
#define CONTINUE_INTERVAL 1.0f
#define SELECT_DURATION 0.5f
#define PLAY_INITIAL_GAP 0.8f

void AMemoryGameActor::BeginPlay()
{
	Super::BeginPlay();

	// Find all actors of the class AMemoryGameActor
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMemoryGameActor::StaticClass(), ActorArray);

	// Cast those actors to an array of type AMemoryGameActor
	for (AActor* Actor : ActorArray)
	{
		if (AMemoryGameActor* MemoryGameActor = Cast<AMemoryGameActor>(Actor))
		{
			MemoryGameActorArray.Add(MemoryGameActor);
		}
	}
}

void AMemoryGameActor::MemoryGameTriggered()
{
	// Start by locking input so the player cannot interrupt any of the timers
	LockInput(true);

	// Create a new game if there isn't one active
	if (!bGameActive)
	{
		// Clear any Animations that are currently active
		ClearAnimation(EClearTimer::All);

		// Generate a random pattern in case there isn't a custom one
		TArray<int32> RandomPattern;
		for (AMemoryGameActor* Actors : MemoryGameActorArray)
		{
			RandomPattern.Add(FMath::RandRange(0, MemoryGameActorArray.Num() - 1));
		}

		StartMemoryGame(RandomPattern);

		// Then play the pattern for the first time
		FTimerHandle ContinueTimer;
		FTimerDelegate ContinueFunction = FTimerDelegate::CreateUFunction(this, FName("ContinueAnimation"), 0);
		GetWorld()->GetTimerManager().SetTimer(ContinueTimer, ContinueFunction, CONTINUE_INITIAL_GAP, false);
	}
	else
	{
		// Play an Animation on the current actor to indicate selection
		SelectAnimation();

		// Set a timer to play the game with that actor selected after the Animation ends
		FTimerHandle PlayTimer;
		GetWorld()->GetTimerManager().SetTimer(PlayTimer, this, &AMemoryGameActor::PlayMemoryGame, PLAY_INITIAL_GAP, false);
	}
}

void AMemoryGameActor::StartMemoryGame(TArray<int32> Pattern)
{
	bGameActive = true;
	CorrectPattern = Pattern;
}

void AMemoryGameActor::PlayMemoryGame()
{
	// Find the index of the current actor in the context of the actor array
	// Then add that index to the current player pattern
	for (int i = 0; i < MemoryGameActorArray.Num(); i++)
	{
		if (MemoryGameActorArray[i] == this)
		{
			PlayerPattern.Add(i);
		}
	}

	// If the pattern is not correct, lose the game
	if (!IsPatternCorrect())
	{
		LoseAnimation();
		EndMemoryGame();
	}
	// If the pattern is correct, and the arrays are the same size, win the game
	else if (PlayerPattern.Num() == CorrectPattern.Num())
	{
		WinAnimation();
		EndMemoryGame();
	}
	// If the player has caught up to the current index in the pattern, continue with the pattern
	else if (CurrentPatternIndex == CurrentContinueIndex)
	{
		// Play the continue animation
		FTimerHandle ContinueTimer;
		FTimerDelegate ContinueFunction = FTimerDelegate::CreateUFunction(this, FName("ContinueAnimation"), 0);
		GetWorld()->GetTimerManager().SetTimer(ContinueTimer, ContinueFunction, CONTINUE_INITIAL_GAP, false);

		// Reset the current player pattern and incriment the continue index
		PlayerPattern.Empty();
		CurrentPatternIndex = 0;
		CurrentContinueIndex++;
	}
	// Otherwise, the player is still entering the current pattern, so incriment the current pattern index
	else
	{
		CurrentPatternIndex++;
		LockInput(false);
	}
}

void AMemoryGameActor::EndMemoryGame()
{
	bGameActive = false;
	CurrentPatternIndex = 0;
	CurrentContinueIndex = 0;
	PlayerPattern.Empty();
	CorrectPattern.Empty();
}

void AMemoryGameActor::SyncMemoryGameActors()
{
	for (AMemoryGameActor* Actors : MemoryGameActorArray)
	{
		Actors->bActiveAnimation = this->bActiveAnimation;
		Actors->bGameActive = this->bGameActive;
		Actors->CurrentPatternIndex = this->CurrentPatternIndex;
		Actors->CurrentContinueIndex = this->CurrentContinueIndex;
		Actors->PlayerPattern = this->PlayerPattern;
		Actors->CorrectPattern = this->CorrectPattern;
	}
}

bool AMemoryGameActor::IsPatternCorrect()
{
	// Check that all values in the arrays match
	// Limit the loop by the player pattern so we only count up to our current index in the pattern
	for (int i = 0; i < PlayerPattern.Num(); i++)
	{
		if (PlayerPattern[i] != CorrectPattern[i])
		{
			return false;
		}
	}

	return true;
}

void AMemoryGameActor::SelectAnimation()
{
	SetGlow(GLOW_INTENSITY);

	FTimerHandle ClearTimer;
	FTimerDelegate ClearFunction = FTimerDelegate::CreateUFunction(this, FName("ClearAnimation"), EClearTimer::Single);
	GetWorld()->GetTimerManager().SetTimer(ClearTimer, ClearFunction, SELECT_DURATION, false);
}

void AMemoryGameActor::ContinueAnimation(int32 ContinueIndex)
{
	if (ContinueIndex < CurrentContinueIndex + 1)
	{
		MemoryGameActorArray[CorrectPattern[ContinueIndex]]->SelectAnimation();

		FTimerHandle ContinueTimer;
		FTimerDelegate ContinueFunction = FTimerDelegate::CreateUFunction(this, FName("ContinueAnimation"), ++ContinueIndex);
		GetWorld()->GetTimerManager().SetTimer(ContinueTimer, ContinueFunction, CONTINUE_INTERVAL, false);
	}
	// Unlock the input only after the entire continue animation has concluded
	else if (ContinueIndex == CurrentContinueIndex + 1)
	{
		FTimerHandle LockTimer;
		FTimerDelegate LockFunction = FTimerDelegate::CreateUFunction(this, FName("LockInput"), false);
		GetWorld()->GetTimerManager().SetTimer(LockTimer, LockFunction, CONTINUE_INTERVAL, false);
	}
}

void AMemoryGameActor::WinAnimation()
{
	for (AMemoryGameActor* Actor : MemoryGameActorArray)
	{
		Actor->StartBlinking(SELECT_DURATION, GLOW_INTENSITY);
	}

	FTimerHandle ClearTimer;
	FTimerDelegate ClearFunction = FTimerDelegate::CreateUFunction(this, FName("ClearAnimation"), EClearTimer::All);
	GetWorld()->GetTimerManager().SetTimer(ClearTimer, ClearFunction, END_DURATION, false);
}

void AMemoryGameActor::LoseAnimation()
{
	for (AMemoryGameActor* Actor : MemoryGameActorArray)
	{
		Actor->SetGlow(GLOW_INTENSITY);
	}

	FTimerHandle ClearTimer;
	FTimerDelegate ClearFunction = FTimerDelegate::CreateUFunction(this, FName("ClearAnimation"), EClearTimer::All);
	GetWorld()->GetTimerManager().SetTimer(ClearTimer, ClearFunction, END_DURATION, false);
}

void AMemoryGameActor::ClearAnimation(const EClearTimer Actors)
{
	if (Actors == EClearTimer::Single)
	{
		StopBlinking();
		SetGlow(0.0f);
	}
	else if (Actors == EClearTimer::All)
	{
		for (AMemoryGameActor* Actor : MemoryGameActorArray)
		{
			Actor->StopBlinking();
			Actor->SetGlow(0.0f);
		}
		
		LockInput(false);
	}
}

void AMemoryGameActor::LockInput(const bool bLocked)
{
	if (bLocked)
	{
		bActiveAnimation = true;
	}
	else
	{
		bActiveAnimation = false;
	}
	
	SyncMemoryGameActors();
}
