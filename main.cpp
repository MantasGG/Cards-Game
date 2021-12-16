#include <iostream>
#include <fstream>
#include <time.h>
#include <queue>
#include <string>

// Doubly circular linked list
#include "./libraries/Cdbll.cpp"
#include "./libraries/Cdbll.h"

using namespace std;

struct kortos{
    string title;
    string action;
    int points;
};

void readCards(int& n, kortos cardsTypes[10]){
    string title, action;
    int points;

    //cout << "Reading cards from file..." << endl;
    ifstream cardsFile("cards.txt");
    cardsFile >> n;
    for(int i=0; i<n; i++){
        cardsFile >> title >> action >> points;
        cardsTypes[i].title = title;
        cardsTypes[i].action = action;
        cardsTypes[i].points = points;
        //cout << title << " " << action << " " << points << endl;
    }
    cardsFile.close();
    //cout << "Done reading." << endl;
}

void GenerateCardDeck(int cardsNumber, queue<int>& deck){
    int cardId;
    int n=20;
    //cout << "Generating shuffeled deck..." << endl;
    srand(time(0));
    for(int i=0;i<n;i++){
        //if(i%5==0) cout << "..." << endl;
        cardId = rand() % cardsNumber;
        //cout << cardId << " ";
        deck.push(cardId);
    }
    //cout << endl;
    //cout << "Done." << endl;
}

void giveCards(int n, Cdbll<int>& holdingCards, queue<int>& deck, int cardsNumber){
    for(int i=0;i<n;i++){
        if(deck.empty()){ 
            //cout << "Out of cards!\nGenerating more cards..." << endl;
            GenerateCardDeck(cardsNumber,deck);
        }
        //cout << "Giving cards... (" << i+1 << ")"<< endl;

        holdingCards.insertEnd(deck.front());
        deck.pop();
        
    }
}

void printGameUI(Cdbll<int> playerCards, kortos cardsTypes[],int playerHp, int botHp){

    cout << "\n     Player hp: " << playerHp << "                          Bot hp: " << botHp << "\n\n       ";

int n = playerCards.length(); // Number of player cards

    for(int i=0;i<n;i++){
        cout << char(201);
        for(int j=0;j<9;j++){
            cout << char(205);
        }
        cout << char(187);
    }

    for(int i=0;i<6;i++){
        cout << "\n       ";
        for(int j=0;j<n;j++){

            if(i==1){
                cout << char(186) << " " << cardsTypes[playerCards.getValue(j)].title;
                int x = 9-cardsTypes[playerCards.getValue(j)].title.length()-1;
                for(int z=0;z<x;z++){
                    cout << " ";
                }
                cout << char(186);
            }

            else if(i==4){
                cout << char(186) << " " << cardsTypes[playerCards.getValue(j)].action;
                int x = 9-cardsTypes[playerCards.getValue(j)].action.length()-1;
                for(int z=0;z<x;z++){
                    cout << " ";
                }
                cout << char(186);
            }

            else if(i==5){
                cout << char(186) << "    " << cardsTypes[playerCards.getValue(j)].points;
                int x = 9-to_string(cardsTypes[playerCards.getValue(j)].points).length()-4;
                for(int z=0;z<x;z++){
                    cout << " ";
                }
                cout << char(186);
            }
            else cout << char(186) << "         " << char(186);
        }
    }
    cout << "\n       ";
    for(int i=0;i<n;i++){
        cout << char(200);
        for(int j=0;j<9;j++){
            cout << char(205);
        }
        cout << char(188);
    }
    cout << "\n       ";
    for(int i=0;i<n;i++){
        cout << "     " << i+1 << "     ";
    }

    cout << endl << endl;
}

void Clear( queue<int> &q )
{
   queue<int> empty;
   swap( q, empty );
}



int main() 
{

    // menu
    cout << "Cards game\n" << endl;
    cout << "             Menu\n" << endl;
    cout << "1 Start game" << endl;
    cout << "2 Quit\n" << endl;
    cout << "Choose: ";
    int menu;
    cin >> menu;
    if(menu==2) exit(0);

    // game

    int playerHp;
    int botHp;
    int cardsNumber;
    int botChoice;

    bool playAgain = true;
    char again;

    queue<int> deck; // queue
    Cdbll<int> playerCards; // doubly circular linked list
    Cdbll<int> botCards; // doubly circular linked list
    
    kortos cardsTypes[20]; // cards structure


    readCards(cardsNumber,cardsTypes);

    int x;
    int botCard=-1;
    int playerCard=-1;

    while(playAgain){
        x=0;
        playerHp=20;
        botHp=20;

        botCard=-1;
        playerCard=-1;
        
        Clear(deck);
        GenerateCardDeck(cardsNumber,deck);

        while(x!=-1){
            playerCards.emptylist();
            botCards.emptylist();
            // Player move
            giveCards(4,playerCards,deck,cardsNumber);
            giveCards(4,botCards,deck,cardsNumber);

            system("cls");
            printGameUI(playerCards, cardsTypes,playerHp,botHp);

            if(playerCard!=-1) cout << "Player used card '" << cardsTypes[playerCard].title << "' which has effect of " << cardsTypes[playerCard].points << " " << cardsTypes[playerCard].action << endl;
            if(botCard!=-1) cout << "Bot used card '" << cardsTypes[botCard].title << "' which has effect of " << cardsTypes[botCard].points << " " << cardsTypes[botCard].action << endl;

            cout << "\nChoose card (to play again type -1 / to exit type -2): ";
            cin >> x;

            if(x>playerCards.length()){
                while(x>playerCards.length()){
                    cout << "Card not existing, choose another card (to play again type -1 / to exit type -2): ";
                    cin >> x;
                }
            }

            if(x==-2) exit(0);

            playerCard=playerCards.getValue(x-1);
            if(cardsTypes[playerCard].action == "damage"){
                botHp-=cardsTypes[playerCard].points;
            }

            else{
                playerHp+=cardsTypes[playerCard].points;
            }

            cout << "Player used card '" << cardsTypes[playerCard].title << "' which have effect of " << cardsTypes[playerCard].points << " " << cardsTypes[playerCard].action << endl;
            //playerCards.deleteItem(playerCard);

            if(botHp<=0){
                while(1){
                system("cls");
                cout << "\n\n\n                     Player won against bot!\n\n\n" << endl;
                    cout << "Play again? (y/n) ";
                    cin >> again;
                    if(again == 'y') {
                        playAgain=true;
                        break;
                    }
                    else if(again == 'n') {
                        playAgain=false;
                        break;
                    }
                }
                break;
            }

            // Bot move
            botChoice = 20 + rand() % 30+1;
            botCard = botCards.getValue(botChoice-1);
            if(cardsTypes[botCard].action == "damage"){
                playerHp-=cardsTypes[botCard].points;
            }
            else{
                botHp+=cardsTypes[botCard].points;
            }

            if(playerHp<=0){
                while(1){
                system("cls");
                cout << "\n\n\n                     Bot won against player!\n\n\n" << endl;
                cout << "Play again? (y/n) ";
                    cin >> again;
                    if(again == 'y') {
                        playAgain=true;
                        break;
                    }
                    else if(again == 'n') {
                        playAgain=false;
                        break;
                    }
                }
                break;
            }
        }
    }

    system("pause");
}