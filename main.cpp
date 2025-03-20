#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>

using namespace std;

#define MAX_PLAYERS 10
#define MAX_ROUNDS 10
#define MAX_ATTEMPTS 3

// Frasi predefinite
const char* phrases[] = {
    "maurizio benazzi, è dal giorno zero che vivi succhiando cazzi",
    "il rosso ti chiede di scoparlo perchè sa che sei ricchione",
    "una severa quanto onesta punizione",
    "il rosso si troverà presto un nuovo amante",
    "uno come il benaz è difficile da trovare",
    "ma ci devi ancora un mc, brutto pezzente",
    "Ok ok, qui si parla di nuovo del ritardato",
    "Quando morirai solo la apple sarà in lutto",
    "Mauri mauri spendaccione",
    "Maiale è certamente più appropriato",
    "Poi metti il colpo in Canna e punta alla gola",
    "Il tuo corpo dai corvi divorato",
    "Posso dire con certezza che avevamo ragione",
    "Dici che il rosso è un grande amante della figa",
    "Il giorno del funerale non sarò dispiaciuto",
    "Venderai il culo per non rimanere senza un tetto",
    "Hanno sempre addosso tutti gli occhi",
    "Al sabato sera esci con la tua puttana",
    "Sei così grasso che morirai a quarant’anni infartato",
    "Ormai è diventato un bersaglio mirato",
    "Ci siamo rotto in culo, il giorno è arrivato",
    "Quello che dice quanto sei micro dotato",
    "I carabinieri ti troveranno già sezionato",
    "Dopo il botto verrai coperto di sputo",
    "Se ci avessi chiesto scusa ti saresti salvato",
    "Per ammazzarti ho avuto mille motivi",
    "E così siamo arrivati al giorno del giudizio",
    "La sua testa finirà in questa busta",
    "La spedizione parte, le armi sono affilate",
    "Pensando a come faremo fuori quel ratto",
    "Nemmeno dio lo potrà salvare",
    "Ora è il momento di ammazzare",
    "Apriamo la porta le luci sono spente",
    "Sul suo volto compare un’espressione impaurita",
    "Coglione non muoverti, ormai sei fottuto",
    "Quella dove parlo della sua dipartita",
    "Il ciccione è sul tavolo, ha smesso di lamentarsi",
    "In questo modo non ci sarà rumore",
    "Il battito è fermo, è proprio morto",
    "Tanti sacchi neri per le parti di cadavere",
    "Il peso è comunque difficile da smuovere",
    "I genitori non denunceranno l’ accaduto"
};
const int num_phrases = sizeof(phrases) / sizeof(phrases[0]);

struct Player {
    char name[50];
    int score;
};

void mask_last_word(const char* phrase, char* masked_phrase) {
    strcpy(masked_phrase, phrase);
    char* last_space = strrchr(masked_phrase, ' ');
    if (last_space) {
        last_space++; // Spostarsi sulla prima lettera dell'ultima parola
        memset(last_space, '*', strlen(last_space));
    }
}

int main() {
    int num_players, num_rounds;
    Player players[MAX_PLAYERS];

    // Input numero giocatori
    cout << "Inserisci il numero di giocatori: ";
    cin >> num_players;

    for (int i = 0; i < num_players; i++) {
        cout << "Inserisci il nome del giocatore " << i + 1 << ": ";
        cin >> players[i].name;
        players[i].score = 0;
    }

    cout << "Inserisci il numero di round: ";
    cin >> num_rounds;

    srand(time(0));

    for (int round = 0; round < num_rounds; round++) {
        for (int i = 0; i < num_players; i++) {
            cout << "\nRound " << round + 1 << " - Turno di " << players[i].name << "\n";

            int phrase_index = rand() % num_phrases;
            char masked_phrase[100];
            mask_last_word(phrases[phrase_index], masked_phrase);

            cout << "Frase: " << masked_phrase << "\n";

            char correct_word[50];
            sscanf(strrchr(phrases[phrase_index], ' ') + 1, "%s", correct_word);

            bool guessed = false;
            for (int attempt = 0; attempt < MAX_ATTEMPTS && !guessed; attempt++) {
                cout << "Tentativo " << attempt + 1 << ": ";
                char user_guess[50];
                cin >> user_guess;

                if (strcmp(user_guess, correct_word) == 0) {
                    int points = (attempt == 0) ? 10 : (attempt == 1) ? 5 : 3;
                    players[i].score += points;
                    cout << "Corretto! Hai guadagnato " << points << " punti.\n";
                    guessed = true;
                }
                else if (strcmp(user_guess, correct_word) != 0) {
                    cout << "Sbagliato!\n";
                    if ( attempt == 2 ) {
                        cout <<" La parola era: " << correct_word << " ahahah gay" ;
                    }
                }
            }
        }

        // Mostra la classifica dopo ogni round
        cout << "\nClassifica dopo il round " << round + 1 << ":\n";
        for (int i = 0; i < num_players; i++) {
            cout << players[i].name << ": " << players[i].score << " punti\n";
        }
    }

    // Determina il vincitore
    Player winner = players[0];
    for (int i = 1; i < num_players; i++) {
        if (players[i].score > winner.score) {
            winner = players[i];
        }
    }

    // Fine del gioco
    cout << "\nClassifica finale:\n";
    for (int i = 0; i < num_players; i++) {
        cout << players[i].name << ": " << players[i].score << " punti\n";
    }

    cout << "\nIl vincitore è: " << winner.name << " con " << winner.score << " punti!\n";

    cout << "Grazie per aver giocato!";
    return 0;
}