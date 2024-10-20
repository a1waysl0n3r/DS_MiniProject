#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h>
#include <windows.h>

const char *message[] = {"You are in Starting area!","You just entered the puzzle room!",
                    "You encountered a fight","Be prepared for the Mini Boss!","You just found some treasure !",
                    "The eternal doom is approaching","You are in rest area! Purchase what you want!",};

struct tool{
    const char* name;
    int dmg;
    int bleed;
    int accuracy;
    int damp;
};
typedef struct tool tool;
tool short_sword = {"Dagger", 8, 5, 0, 1};
tool bow = {"Bow", 10, 0, 20, 0 };
tool long_sword = {"Crescent Cleaver", 15, 0 , -15, 10};

struct room{
    int room_type;
    struct room* north;
    struct room* south;
    struct room* east;
    struct room* west;
};

typedef struct room* room;

struct player{
    struct room* head;
    int max_hp;
    int hp;
    int dmg;
    int bleed;
    int accuracy;
    int defense;
    struct tool* tool;
};

struct player player = {NULL, 100, 100, 0, 0, 50, 5, NULL};

struct entities{
    int hp;
    int dmg;
    int accuracy;
};

struct entities mob_1 = {150,7,40};
struct entities mob_2 = {130,8,55};
room createroom(int type){
    struct room* new_room = (struct room*)malloc(sizeof(struct room));
    new_room->north = new_room->south = new_room->east = new_room-> west = NULL;
    new_room->room_type = type;
    return new_room;
}

void connectRooms(struct room *current, struct room *n, struct room *s, struct room *w, struct room *e) {
    current->north = n;
    current->south = s;
    current->west = w;
    current->east = e;
}



bool isValidInput(int number) {
    // Check if the number is 3, 2, or 5
    return (number == 3 || number == 2 || number == 5);
}

bool puzzle_1();
bool puzzle_2();

void puzzle(){
    int num = rand() % 10;
    bool cond;
    if(num <= 5){
        puzzle_2();
        return;
    }
    else{
        puzzle_1();
        return;
    }
}


void miniboss_shadow_assassin(struct entities mob) {
    srand(time(NULL));
    bool invisible = false;
    int turn_counter = 0;
    printf("You encountered the Shadow Assassin!\n");

    while (mob.hp > 0 && player.hp > 0) {
        turn_counter++;

        if (turn_counter % 3 == 0) {
            invisible = true;
            printf("The Shadow Assassin has cloaked itself!\n");
        }

        int playerChoice;
        printf("Choose an action:\n1. Attack\n2. Defend\n");
        scanf("%d", &playerChoice);

        if (playerChoice == 1) {
            if (!invisible) {
                int prob = hit_probability();
                if (prob < player.accuracy) {
                    mob.hp -= player.dmg + player.tool->dmg;
                    printf("You hit the Shadow Assassin for %d damage! HP: %d\n", player.dmg + player.tool->dmg, mob.hp);
                } else {
                    printf("You missed!\n");
                }
            } else {
                printf("You can't hit the Shadow Assassin while it's cloaked!\n");
            }
        } else if (playerChoice == 2) {
            printf("You defended against the mob's attack!\n");
            continue;
        } else {
            printf("Invalid choice.\n");
            continue;
        }

        if (mob.hp <= 0) {
            printf("You defeated the Shadow Assassin!\n");
            player.max_hp += 10;
            break;
        }

        // Mob's turn to attack
        if (!invisible) {
            int mobProb = hit_probability();
            if (mobProb < mob.accuracy) {
                player.hp -= mob.dmg;
                printf("The Shadow Assassin hit you for %d damage! Your HP: %d\n", mob.dmg, player.hp);
            } else {
                printf("The Shadow Assassin missed!\n");
            }
        }

        // Reset invisibility for the next round
        if (invisible) {
            invisible = false;
        }

        if (player.hp <= 0) {
            printf("You have been defeated!\n");
            break;
        }
    }
}

void miniboss_elemental_guardian(struct entities mob) {
    printf("mob hp: %d\nmob dmg: %d\n", mob.hp, mob.dmg);
    int elemental_type = 0; // 0: Fire, 1: Ice, 2: Earth
    printf("You encountered the Elemental Guardian!\n");

    while (mob.hp > 0 && player.hp > 0) {
        elemental_type = rand() % 3; // Randomly change element each turn
        printf("The Elemental Guardian is now %s!\n", (elemental_type == 0) ? "Fire" : (elemental_type == 1) ? "Ice" : "Earth");

        int playerChoice;
        printf("Choose an action:\n1. Attack\n2. Defend\n");
        scanf("%d", &playerChoice);

        if (playerChoice == 1) {
            int prob = hit_probability();
            if (prob < player.accuracy) {
                int damageDealt = player.dmg + player.tool->dmg;

                // Apply elemental effects
                if (elemental_type == 0) {
                    // Fire - Burn effect
                    printf("You hit the Elemental Guardian for %d damage! It is burned!\n", damageDealt);
                    mob.hp -= damageDealt;
                    player.hp -= 3;
                } else if (elemental_type == 1) {
                    // Ice - Slow effect
                    printf("You hit the Elemental Guardian for %d damage! Your speed is decreased!\n", damageDealt);
                    mob.hp -= damageDealt;
                    mob.accuracy += 10;
                } else if (elemental_type == 2) {
                    // Earth - Defense boost
                    printf("You hit the Elemental Guardian for %d damage, but it is defending!\n", damageDealt / 2);
                    mob.hp -= damageDealt / 2; // Reduced damage due to defense
                }
            } else {
                printf("You missed!\n");
            }
        } else if (playerChoice == 2) {
            printf("You defended against the Guardian's attack!\n");
            continue;
        } else {
            printf("Invalid choice.\n");
            continue;
        }

        if (mob.hp <= 0) {
            printf("You defeated the Elemental Guardian!\n");
            player.max_hp += 10;
            break;
        }

        // Guardian's turn to attack
        int mobProb = hit_probability();
        if (mobProb < mob.accuracy) {
            player.hp -= mob.dmg;
            printf("The Elemental Guardian hit you for %d damage! Your HP: %d\n", mob.dmg, player.hp);
        } else {
            printf("The Elemental Guardian missed!\n");
        }

        if (player.hp <= 0) {
            printf("You have been defeated!\n");
            player.head = NULL;
            break;
        }
    }
}

void miniboss(){
    int boss_num = rand() % 2;
    if(boss_num == 0){
        miniboss_elemental_guardian(mob_1);
    }
    else{
        miniboss_shadow_assassin(mob_2);
    }

}

void treasure(){
    ;
}

int hit_probability(){
    int prob = rand() % 100;
    return prob;
}

void boss_fight() {
    struct entities boss = {200, 10, 70}; // Boss properties
    bool hit = false;

    while (boss.hp > 0) {
        if (hit) {
            boss.hp -= player.bleed;
        }
        int choice = actions();
        int prob;
        switch (choice) {
            case 1:
                prob = hit_probability();
                if (prob < player.accuracy + player.tool->accuracy) {
                    boss.hp -= player.dmg + player.tool->dmg;
                    hit = true;
                    printf("You hit the boss for %d damage! Boss HP: %d\n", player.dmg + player.tool->dmg, boss.hp);
                } else {
                    printf("You missed!\n");
                }
                break;
            case 2:
                printf("You defended!\n");
                continue;
            default:
                printf("Invalid choice.\n");
                continue;
        }

        if (boss.hp <= 0) {
            printf("You have defeated the boss!\n");
            break;
        }

        // Boss's turn to attack
        if (hit_probability() < boss.accuracy) {
            player.hp -= boss.dmg;
            printf("The boss hit you for %d damage! Your HP: %d\n", boss.dmg, player.hp);
        } else {
            printf("The boss missed!\n");
        }

        if (player.hp <= 0) {
            printf("You have been defeated by the boss!\n");
            break;
        }
    }
}

void fight(){
    printf("You have encountered a goblin!\n");
    struct entities goblin = {50, 5, 65}; // Boss properties
    bool hit = false;

    while (goblin.hp > 0) {
        if (hit) {
            goblin.hp -= player.bleed;
        }
        int choice = actions();
        int prob = hit_probability();
        switch (choice) {
            case 1:
                prob = hit_probability();
                if (prob < player.accuracy + player.tool->accuracy) {
                    goblin.hp -= player.dmg + player.tool->dmg;
                    hit = true;
                    printf("You hit the goblin for %d damage! Goblin HP: %d\n", player.dmg + player.tool->dmg, goblin.hp);
                } else {
                    printf("You missed!\n");
                }
                break;
            case 2:
                printf("You defended!\n");
                continue;
            case 3:
                printf("You chose to flee!\n");
                return;
            default:
                printf("Invalid choice.\n");
                continue;
        }

        if (goblin.hp <= 0) {
            printf("You have defeated the goblin!\n");
            player.max_hp  += 5;
            break;
        }

        // Boss's turn to attack
        if (hit_probability() < goblin.accuracy) {
            player.hp -= goblin.dmg;
            printf("The goblin hit you for %d damage! Your HP: %d\n", goblin.dmg, player.hp);
        } else {
            printf("The goblin missed!\n");
        }

        if (player.hp <= 0) {
            printf("You have been defeated by the goblin!\n");
            break;
        }
    }
}

void start_scene() {
    printf("Pick a weapon!\n1. Long Sword     2. Dagger      3. Bow\n");
    int choice;
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            player.tool = &long_sword;
            break;
        case 2:
            player.tool = &short_sword;
            break;
        case 3:
            player.tool = &bow;
            break;
        default:
            printf("Invalid choice, defaulting to Long Sword.\n");
            player.tool = &long_sword;
            break;
    }
}

int actions(){
    int action;
    bool flag = true;

    while(flag){
        printf("Select an action: \n");
        printf("1. Hit    2. Block\n");
        printf("Action: ");
        scanf("%d", &action);
        if(action > 2 || action < 1){
            printf("Invalid action\n");
            continue;
        }
        else{
            return action;
        }
    }

    return action;
}

void movePlayer(struct player* player) {
    int choice;
    printf("Choose a direction to move (1: North, 2: South, 3: East, 4: West): ");
    scanf("%d", &choice);
    switch (choice) {
        case 1: // Move north
            if (player->head->north != NULL) {
                player->head = player->head->north;
                printf("Moved north.\n");
            } else {
                printf("Can't move north; there's a wall.\n");
            }
            break;
        case 2: // Move south
            if (player->head->south != NULL) {
                player->head = player->head->south;
                printf("Moved south.\n");
            } else {
                printf("Can't move south; there's a wall.\n");
            }
            break;
        case 3: // Move east
            if (player->head->east != NULL) {
                player->head = player->head->east;
                printf("Moved east.\n");
            } else {
                printf("Can't move east; there's a wall.\n");
            }
            break;
        case 4: // Move west
            if (player->head->west != NULL) {
                player->head = player->head->west;
                printf("Moved west.\n");
            } else {
                printf("Can't move west; there's a wall.\n");
            }
            break;
        default:
            printf("Invalid choice! Please enter a number between 1 and 4.\n");
            break;
    }
}

void rest(){
    int q = 2;
    while(q--){
        printf("Enter a choice:\n1. Full heal\n2. DMG+10\n3. ACC+10\n4. MAX HP+20");
        int choice;
        scanf("%d", &choice);
        switch(choice){
            case 1:
                player.hp = player.max_hp;
                break;
            case 2:
                player.dmg += 10;
                break;
            case 3:
                player.accuracy += 10;
                break;
            case 4:
                player.max_hp += 10;
        }
    }
}

int main(){
    srand(time(NULL));

    room start = createroom(0);

    room fight_1 = createroom(2);
    room fight_2 = createroom(2);
    room fight_3 = createroom(2);

    room treasure_room = createroom(4);

    room puzzle_room_1 = createroom(1);
    room puzzle_room_2 = createroom(1);

    room mini_boss_1 = createroom(3);
    room mini_boss_2 = createroom(3);

    room rest_area = createroom(6);

    room boss = createroom(5);


    //void connectRooms(struct Room *current, struct Room *north, struct Room *south, struct Room *west, struct Room *east
    connectRooms(start, fight_1, treasure_room, NULL, puzzle_room_1);
    connectRooms(fight_1, NULL, NULL, NULL, fight_2);
    connectRooms(fight_2, NULL, NULL, NULL, puzzle_room_2);
    connectRooms(puzzle_room_2, NULL, fight_3, NULL, mini_boss_2);
    connectRooms(mini_boss_2, NULL, NULL, NULL, rest_area);
    connectRooms(rest_area, NULL, NULL, NULL, boss);
    connectRooms(puzzle_room_1, fight_2, NULL, NULL, puzzle_room_2);
    connectRooms(treasure_room, NULL, NULL, NULL, mini_boss_1);
    connectRooms(mini_boss_1, puzzle_room_1, NULL, NULL, fight_3);
    connectRooms(fight_3, puzzle_room_2, NULL, NULL, mini_boss_2);

    player.head = start;
    while(1){
        int current = player.head->room_type;
        if(current == 0){
            start_scene();
            printf("------------------\n");
            movePlayer(&player.head);
            printf("------------------\n");
        }
        else if(current == 1){
            puzzle();
            printf("------------------\n");
            movePlayer(&player.head);
            printf("------------------\n");
        }
        else if(current == 2){
            fight();
            printf("------------------\n");
            movePlayer(&player.head);
            printf("------------------\n");
        }
        else if(current == 3){
            miniboss();
            printf("------------------\n");
            movePlayer(&player.head);
            printf("------------------\n");
        }
        else if(current == 4){
            treasure();
            printf("------------------\n");
            movePlayer(&player.head);
            printf("------------------\n");
        }
        else if(current == 5){
            boss_fight();
            printf("------------------\n");
            movePlayer(&player.head);
            printf("------------------\n");
        }
        else if(current == 6){
            rest();
            printf("------------------\n");
            movePlayer(&player.head);
            printf("------------------\n");
        }
    }

}


bool puzzle_1(){
    char input[100];
    int numbers[3];
    int sum = 0;
    bool validInput;

;

    // Prompt the user for input
    printf("Enter up to three numbers (only 3, 2, or 5) separated by spaces: ");
    scanf("%d %d %d", &numbers[0], &numbers[1], &numbers[2]);

    // Parse the input and count how many numbers were entered


    // Validate each entered number



    // If input is valid, print the numbers
    for (int i = 0; i < 3; i++) {
        sum = sum+numbers[i];
    }
    if(sum != 10){
        printf("\n You failed lose 5 hp");
        player.hp = player.hp - 5;
    }
    return false;

}

bool puzzle_2() {
    // Define the color and number mappingss
    srand(time(NULL));
    const char* colors[] = {"Red", "Green", "Blue", "Yellow", "Purple"};
    int color_count = sizeof(colors) / sizeof(colors[0]);
    for(int i = 0; i < 5; i++){
        printf("%s ", colors[i]);
    }

    Sleep(1000);
    system("cls");
    // Randomly select a color
    int selected_color_index = rand() % color_count;

    // Prompt the player for the color
    char input[6];
    printf("Match the color! What color is associated with number %d?\n", selected_color_index + 1);
    scanf("%s", input);
    // Remove newline character from input

    // Check if the player’s input matches the selected color
    if (strcasecmp(input, colors[selected_color_index]) == 0) {
        printf("Correct! You passed the puzzle.\n");
        player.max_hp += 5;
        return true; // Puzzle passed
    } else {
        printf("Incorrect! The correct color was %s. You lose 5 HP.\n", colors[selected_color_index]);
        player.hp -= 5; // Deduct health for wrong answer
        return false; // Puzzle failed
    }
}


