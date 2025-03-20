#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <thread>  // Per il delay
#include <chrono>  // Per il delay

using namespace std;

#define MAX_PLAYERS 10
#define MAX_ROUNDS 10
#define MAX_ATTEMPTS 3
#define TIME_LIMIT 30 // Tempo massimo per turno in secondi

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
    "Maiale è certamente più appropriato"
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
                cout << "Tentativo " << attempt + 1 << " - Hai " << TIME_LIMIT << " secondi per rispondere!\n";

                time_t start = time(0), now;
                char user_guess[50];

                // Conto alla rovescia visibile
                while (true) {
                    now = time(0);
                    int seconds_passed = now - start;
                    int time_left = TIME_LIMIT - seconds_passed;

                    cout << "⏳ Tempo rimanente: " << time_left << " secondi...\r";
                    cout.flush();  // Per aggiornare la riga di testo

                    if (time_left <= 0) {
                        cout << "⏳ Tempo scaduto! Nessun punto assegnato.\n";
                        guessed = true;
                        break;
                    }

                    // Controllo se l'utente ha inserito una risposta
                    if (cin.peek() != EOF) {
                        cin >> user_guess;
                        if (strcmp(user_guess, correct_word) == 0) {
                            int points = (attempt == 0) ? 10 : (attempt == 1) ? 5 : 3;
                            players[i].score += points;
                            cout << "✅ Corretto! Hai guadagnato " << points << " punti.\n";
                            guessed = true;
                        } else {
                            cout << "❌ Sbagliato!\n";
                            if (attempt == 2) {
                                cout << "La parola era: " << correct_word << " ahahah gay\n";
                            }
                        }
                        break;
                    }

                    this_thread::sleep_for(chrono::seconds(1)); // Aspetta un secondo
                }
            }
        }

        // Mostra la classifica dopo ogni round
        cout << "\n🏆 Classifica dopo il round " << round + 1 << ":\n";
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
    cout << "\n🏁 Classifica finale:\n";
    for (int i = 0; i < num_players; i++) {
        cout << players[i].name << ": " << players[i].score << " punti\n";
    }

    cout << "\n🎉 Il vincitore è: " << winner.name << " con " << winner.score << " punti!\n";

    cout << "Grazie per aver giocato!";
    return 0;
}
