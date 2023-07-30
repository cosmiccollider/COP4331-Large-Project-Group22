// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MemoryGameActor.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

#define CONTINUE_INITIAL_GAP 2.0f
#define CONTINUE_INTERVAL 1.0f
#define END_DURATION 5.0f
#define GLOW_INTENSITY 10.0f
#define PATTERN_LENGTH 4
#define POST_SELECT_INPUT_GAP 0.6f
#define SELECT_DURATION 0.5f

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
		// Generate a random pattern in case there isn't a custom one
		TArray<uint8> RandomPattern;
		for (int i = 0; i < PATTERN_LENGTH; i++)
		{
			RandomPattern.Add(FMath::RandRange(0, MemoryGameActorArray.Num()));
		}

		StartMemoryGame(RandomPattern);

		// Play the pattern for the first time
		FTimerHandle ContinueTimer;
		FTimerDelegate ContinueFunction = FTimerDelegate::CreateUFunction(this, FName("ContinueAnimation"), 0);
		GetWorld()->GetTimerManager().SetTimer(ContinueTimer, ContinueFunction, CONTINUE_INITIAL_GAP, false);
	}
	else
	{
		// Play an Animation on the current actor to indicate selection
		SelectAnimation();

		// Set a timer to play the game after the animation ends
		FTimerHandle PlayTimer;
		GetWorld()->GetTimerManager().SetTimer(PlayTimer, this, &AMemoryGameActor::PlayMemoryGame, POST_SELECT_INPUT_GAP, false);
	}
}

void AMemoryGameActor::StartMemoryGame(TArray<uint8> Pattern)
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
	// If the player has caught up to the continue index, trigger a continue event
	else if (PlayerPattern.Num() == ContinueIndex)
	{
		// Reset the current player pattern
		PlayerPattern.Empty();

		// Play the continue animation to show the player the continuation of the pattern
		FTimerHandle ContinueTimer;
		FTimerDelegate ContinueFunction = FTimerDelegate::CreateUFunction(this, FName("ContinueAnimation"), 0);
		GetWorld()->GetTimerManager().SetTimer(ContinueTimer, ContinueFunction, CONTINUE_INITIAL_GAP, false);

	}
	// Otherwise, the player is still entering the current pattern, so we unlock input so they can input the next actor
	else
	{
		LockInput(false);
	}
}

void AMemoryGameActor::EndMemoryGame()
{
	bGameActive = false;
	ContinueIndex = 0;
	PlayerPattern.Empty();
	CorrectPattern.Empty();
}

void AMemoryGameActor::SyncMemoryGameActors()
{
	for (AMemoryGameActor* Actors : MemoryGameActorArray)
	{
		Actors->bActiveAnimation = this->bActiveAnimation;
		Actors->bGameActive = this->bGameActive;
		Actors->ContinueIndex = this->ContinueIndex;
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

void AMemoryGameActor::ContinueAnimation(uint8 ActorIndex)
{
	// Increment the continue index on only the first call of the recursive function
	if (ActorIndex == 0)
	{
		ContinueIndex++;
	}

	// While the current actor index is less than the last continue index, recursively play the animation
	if (ActorIndex < ContinueIndex - 1)
	{
		MemoryGameActorArray[CorrectPattern[ActorIndex]]->SelectAnimation();

		FTimerHandle ContinueTimer;
		FTimerDelegate ContinueFunction = FTimerDelegate::CreateUFunction(this, FName("ContinueAnimation"), ++ActorIndex);
		GetWorld()->GetTimerManager().SetTimer(ContinueTimer, ContinueFunction, CONTINUE_INTERVAL, false);
	}
	// After playing the last continue animation, unlock input at a faster rate so the player can immediately submit an input after the animation
	else if (ActorIndex < ContinueIndex)
	{
		MemoryGameActorArray[CorrectPattern[ActorIndex]]->SelectAnimation();

		FTimerHandle LockTimer;
		FTimerDelegate LockFunction = FTimerDelegate::CreateUFunction(this, FName("LockInput"), false);
		GetWorld()->GetTimerManager().SetTimer(LockTimer, LockFunction, POST_SELECT_INPUT_GAP, false);
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
