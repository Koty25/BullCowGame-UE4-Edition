// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
//#include "HiddenWordList.h"

/* What to improve:
1- Ability to quit the game via a simple menu with two options, resume game and quit game;
2- A character model for the PC;
3- Better description of how to use the terminal;
4- Tide up the code and all the commented/debug code*/



void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    FBullCowCount Count;

    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordList/HiddenWordList2.txt");
    FFileHelper::LoadFileToStringArrayWithPredicate(Words, *WordListPath, [](const FString& Word)
    {
        return Word.Len() > 3 && IsIsogram(Word);
    });

    Isograms = GetValidWords(Words);

    //GetValidWords(Words);
    SetupGame(); //Set up the game

    PrintLine(TEXT("Press TAB to interact with the terminal."));
    
    // PrintLine(TEXT("The number of possible Isograms is %i."), Isograms.Num());

    PrintLine(TEXT("The HiddenWord is: %s."), *HiddenWord); //debug line   
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        ProcessGuess(Input);
    }

    
    
}

void UBullCowCartridge::SetupGame()
{
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
    Lives = HiddenWord.Len() + 1;
    bGameOver = false;

    PrintLine(TEXT("Welcome to the Bulls and Cows game!"));
    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("You have %i Lives."), Lives);
    PrintLine(TEXT("Press enter to continue and then input the guess..."));
    PrintGuess(TEXT("| Guess | Bulls | Cows"));

    
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press enter to restart the game or esc to exit."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{


    if (Guess == HiddenWord)
        {
            PrintLine(TEXT("You Win!"));
            EndGame();
            return;
        }

    if (Lives < 1)
    {
        PrintLine(TEXT("You lost."));
        PrintLine(TEXT("Your word was %s."), *HiddenWord);
        EndGame();
        return;
    }

    if (!IsIsogram(Guess))
        {
            PrintLine(TEXT("No repeating letter, guess again!"));
            return;
        }

    if (Guess.Len() != HiddenWord.Len())
        {
            PrintLine(TEXT("The Hidden Word is %i characters long."), HiddenWord.Len());
            // PrintLine(TEXT("Now you have %i Lives"), --Lives);
            return;
        }
    
    PrintLine(TEXT("You guessed the wrong word. \nYou have lost 1 life."));
    PrintLine(TEXT("Now you have %i Lives"), --Lives);

    //int32 Bulls, Cows;

    //GetBullCows(Guess, Bulls, Cows);
    FBullCowCount Score = GetBullCows(Guess);

    //PrintLine(TEXT("You have %i Bulls and %i Cows"), Bulls, Cows);
    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);
    PrintGuess(TEXT("| %s | %i B | %i C"), *FString(Guess), Score.Bulls, Score.Cows);
        
}

bool UBullCowCartridge::IsIsogram(const FString& Word)
{
    for (int32 Comparisson = 0; Comparisson < (Word.Len()-1); Comparisson++)
    {
        for (int32 Index = Comparisson + 1 ; Index < Word.Len(); Index++)
        {
            if (Word[Comparisson] == Word[Index])
            {
                return false;
            }
            
        }
    }
    
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{

    TArray<FString> ValidWords;

    for (FString IsoWord : WordList)
    {
        if (IsoWord.Len() > 3 && IsIsogram(IsoWord))
        {
            ValidWords.Emplace(IsoWord);
        }
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    for (int32 BullIndex = 0; BullIndex < Guess.Len(); BullIndex++)
    {
        if (Guess[BullIndex] == HiddenWord[BullIndex])
        {
            Count.Bulls++;
            continue;
        }
        for (int32 CowIndex = 0; CowIndex < HiddenWord.Len(); CowIndex++)
        {
            if (Guess[BullIndex] == HiddenWord[CowIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}

// void UBullCowCartridge::GetBullCows(const FString& Guess, int32& BullCount, int32& CowCount) const
// {
//     BullCount = 0;
//     CowCount = 0;

//     for (int32 BullIndex = 0; BullIndex < Guess.Len(); BullIndex++)
//     {
//         if (Guess[BullIndex] == HiddenWord[BullIndex])
//         {
//             BullCount++;
//             continue;
//         }
//         for (int32 CowIndex = 0; CowIndex < HiddenWord.Len(); CowIndex++)
//         {
//             if (Guess[BullIndex] == HiddenWord[CowIndex])
//             {
//                 CowCount++;
//                 break;
//             }
//         }
//     }
// }