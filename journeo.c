#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50
#define HASH_SIZE 10

/* ---------------------------------
   DATA STRUCTURES
----------------------------------*/

typedef struct Place {
    char name[50];
    char location[50];
    char type[50];
    float rating;
    struct Place *next;
} Place;

typedef struct Interest {
    char name[50];
    Place *places;
    struct Interest *next;
} Interest;

typedef struct State {
    char name[50];
    Interest *interests;
    struct State *next;
} State;

/* Queue */
typedef struct {
    Place *items[MAX];
    int front, rear;
} Queue;

/* Hash table */
typedef struct HashNode {
    Place *place;
    struct HashNode *next;
} HashNode;

/* ---------------------------------
   GLOBALS
----------------------------------*/
State *state_head = NULL;
HashNode *htable[HASH_SIZE] = {0};
Queue trending;

/* ---------------------------------
   UTILS
----------------------------------*/

void get_input(char *str, int size) {
    fgets(str, size, stdin);
    str[strcspn(str, "\n")] = 0;
}

int hash(char *str) {
    int sum = 0;
    int i;
    for (i = 0; str[i]; i++)
        sum += str[i];
    return sum % HASH_SIZE;
}

/* Queue operations */
void enqueue(Place *p) {
    if ((trending.rear + 1) % MAX == trending.front)
        return;
    trending.items[trending.rear] = p;
    trending.rear = (trending.rear + 1) % MAX;
}

Place* dequeue() {
    if (trending.front == trending.rear)
        return NULL;
    Place *p = trending.items[trending.front];
    trending.front = (trending.front + 1) % MAX;
    return p;
}

/* Hash table insert */
void insert_hash(Place *p) {
    int idx = hash(p->location);
    HashNode *node = malloc(sizeof(HashNode));
    node->place = p;
    node->next = htable[idx];
    htable[idx] = node;
}

/* ---------------------------------
   CREATION FUNCTIONS
----------------------------------*/

State* add_state(char *name) {
    State *s = malloc(sizeof(State));
    strcpy(s->name, name);
    s->interests = NULL;
    s->next = state_head;
    state_head = s;
    return s;
}

Interest* add_interest(State *s, char *name) {
    Interest *i = malloc(sizeof(Interest));
    strcpy(i->name, name);
    i->places = NULL;
    i->next = s->interests;
    s->interests = i;
    return i;
}

Place* add_place(Interest *i, char *name, char *loc, char *type, float rating) {
    Place *p = malloc(sizeof(Place));
    strcpy(p->name, name);
    strcpy(p->location, loc);
    strcpy(p->type, type);
    p->rating = rating;

    p->next = i->places;
    i->places = p;

    enqueue(p);
    insert_hash(p);

    return p;
}

/* ---------------------------------
   DISPLAY FUNCTIONS
----------------------------------*/

void show_states() {
    printf("\nAvailable States:\n");
    State *s;
    for (s = state_head; s; s = s->next)
        printf("- %s\n", s->name);
}

void show_interests(State *s) {
    printf("\nInterests in %s:\n", s->name);
    Interest *i;
    for (i = s->interests; i; i = i->next)
        printf("- %s\n", i->name);
}

void show_places(Interest *i) {
    printf("\nPlaces under %s:\n", i->name);
    Place *p;
    for (p = i->places; p; p = p->next)
        printf("%s (%s) - %.1f?\n", p->name, p->location, p->rating);
}

void show_trending() {
    printf("\nTrending Places:\n");

    Queue temp = trending;
    Place *p;

    while ((p = dequeue()) != NULL) {
        printf("- %s (%s) - %.1f?\n", p->name, p->location, p->rating);
    }
    trending = temp;
}

void search_by_location(char *loc) {
    int idx = hash(loc);
    HashNode *temp = htable[idx];
    int found = 0;

    printf("\nResults for \"%s\":\n", loc);

    while (temp) {
        if (strcmp(temp->place->location, loc) == 0) {
            printf("- %s (%s) - %.1f?\n",
                temp->place->name,
                temp->place->type,
                temp->place->rating);
            found = 1;
        }
        temp = temp->next;
    }

    if (!found)
        printf("No places found here.\n");
}

/* ---------------------------------
   INITIAL DATA
----------------------------------*/

void init_data() {

    /* ANDHRA PRADESH */
    State *ap = add_state("Andhra Pradesh");

    Interest *ap_beach = add_interest(ap, "Beaches");
    add_place(ap_beach, "RK Beach", "Vizag", "Beach", 4.3);
    add_place(ap_beach, "Rishikonda Beach", "Vizag", "Beach", 4.5);
    add_place(ap_beach, "Bheemili Beach", "Vizag", "Beach", 4.2);

    Interest *ap_temple = add_interest(ap, "Temples");
    add_place(ap_temple, "Simhachalam Temple", "Vizag", "Temple", 4.7);
    add_place(ap_temple, "Kanaka Durga Temple", "Vijayawada", "Temple", 4.6);
    add_place(ap_temple, "Annavaram Temple", "East Godavari", "Temple", 4.5);

    Interest *ap_hill = add_interest(ap, "Hill Stations");
    add_place(ap_hill, "Araku Valley", "Visakhapatnam", "Hill Station", 4.8);
    add_place(ap_hill, "Horsley Hills", "Chittoor", "Hill Station", 4.4);

    Interest *ap_city = add_interest(ap, "City Life");
    add_place(ap_city, "Vijayawada", "Vijayawada", "City", 4.2);
    add_place(ap_city, "Vizag City", "Vizag", "City", 4.3);

    /* TAMIL NADU */
    State *tn = add_state("Tamil Nadu");

    Interest *tn_heritage = add_interest(tn, "Heritage");
    add_place(tn_heritage, "Mahabalipuram", "Chennai", "Heritage Site", 4.8);
    add_place(tn_heritage, "Brihadeeswarar Temple", "Thanjavur", "Temple", 4.9);
    add_place(tn_heritage, "Gangaikonda Cholapuram", "Ariyalur", "Heritage Site", 4.7);

    Interest *tn_beach = add_interest(tn, "Beaches");
    add_place(tn_beach, "Marina Beach", "Chennai", "Beach", 4.4);
    add_place(tn_beach, "Elliot's Beach", "Chennai", "Beach", 4.3);

    Interest *tn_temple = add_interest(tn, "Temples");
    add_place(tn_temple, "Meenakshi Amman Temple", "Madurai", "Temple", 4.9);
    add_place(tn_temple, "Ramanathaswamy Temple", "Rameshwaram", "Temple", 4.8);

    Interest *tn_hill = add_interest(tn, "Hill Stations");
    add_place(tn_hill, "Ooty", "Nilgiris", "Hill Station", 4.6);
    add_place(tn_hill, "Kodaikanal", "Dindigul", "Hill Station", 4.7);

    /* KARNATAKA */
    State *ka = add_state("Karnataka");

    Interest *ka_hill = add_interest(ka, "Hill Stations");
    add_place(ka_hill, "Coorg", "Kodagu", "Hill Station", 4.8);
    add_place(ka_hill, "Chikmagalur", "Chikmagalur", "Hill Station", 4.7);

    Interest *ka_temple = add_interest(ka, "Temples");
    add_place(ka_temple, "Murudeshwar Temple", "Uttara Kannada", "Temple", 4.8);
    add_place(ka_temple, "Udupi Sri Krishna", "Udupi", "Temple", 4.6);

    Interest *ka_wild = add_interest(ka, "Wildlife");
    add_place(ka_wild, "Bandipur National Park", "Chamarajnagar", "Wildlife", 4.7);
    add_place(ka_wild, "Nagarhole National Park", "Kodagu", "Wildlife", 4.6);

    Interest *ka_city = add_interest(ka, "City Life");
    add_place(ka_city, "Bengaluru", "Bengaluru", "City", 4.6);
    add_place(ka_city, "Mysuru", "Mysuru", "City", 4.5);

    /* TELANGANA */
    State *ts = add_state("Telangana");

    Interest *ts_heritage = add_interest(ts, "Heritage");
    add_place(ts_heritage, "Charminar", "Hyderabad", "Monument", 4.7);
    add_place(ts_heritage, "Golconda Fort", "Hyderabad", "Fort", 4.6);
    add_place(ts_heritage, "Ramappa Temple", "Mulugu", "Temple", 4.8);

    Interest *ts_temple = add_interest(ts, "Temples");
    add_place(ts_temple, "Yadadri Temple", "Yadadri", "Temple", 4.8);
    add_place(ts_temple, "Birla Mandir", "Hyderabad", "Temple", 4.6);
    add_place(ts_temple, "Bhadrachalam Temple", "Bhadrachalam", "Temple", 4.7);

    Interest *ts_hill = add_interest(ts, "Hill Stations");
    add_place(ts_hill, "Ananthagiri Hills", "Vikarabad", "Hill Station", 4.5);
    add_place(ts_hill, "Bogatha Waterfalls", "Mulugu", "Waterfall", 4.6);

    Interest *ts_city = add_interest(ts, "City Life");
    add_place(ts_city, "Hyderabad", "Hyderabad", "City", 4.8);
    add_place(ts_city, "Warangal City", "Warangal", "City", 4.5);

    Interest *ts_food = add_interest(ts, "Food Streets");
    add_place(ts_food, "Paradise Biryani", "Secunderabad", "Restaurant", 4.4);
    add_place(ts_food, "Shah Ghouse", "Hyderabad", "Restaurant", 4.5);
    add_place(ts_food, "Nampally Bazar", "Hyderabad", "Street Market", 4.3);

    /* GOA */
    State *goa = add_state("Goa");

    Interest *goa_beach = add_interest(goa, "Beaches");
    add_place(goa_beach, "Baga Beach", "Baga", "Beach", 4.6);
    add_place(goa_beach, "Calangute Beach", "Calangute", "Beach", 4.5);
    add_place(goa_beach, "Palolem Beach", "Palolem", "Beach", 4.8);

    Interest *goa_church = add_interest(goa, "Churches & Heritage");
    add_place(goa_church, "Basilica of Bom Jesus", "Old Goa", "Church", 4.8);
    add_place(goa_church, "Se Cathedral", "Old Goa", "Church", 4.7);

    Interest *goa_hill = add_interest(goa, "Nature & Hills");
    add_place(goa_hill, "Dudhsagar Waterfalls", "Sonaulim", "Waterfall", 4.9);

    Interest *goa_city = add_interest(goa, "City Life");
    add_place(goa_city, "Panaji", "Panaji", "City", 4.6);
    add_place(goa_city, "Margao", "Margao", "City", 4.4);
}

/* ---------------------------------
   MAIN PROGRAM
----------------------------------*/

int main() {
    trending.front = trending.rear = 0;
    init_data();

    int choice;
    char input[50], in[50];

    while (1) {
        printf("\n=== JOURNEO: INTEREST-BASED TOUR GUIDE ===\n");
        printf("1. Show States\n");
        printf("2. Show Interests in a State\n");
        printf("3. Show Places for an Interest\n");
        printf("4. Show Trending Places\n");
        printf("5. Search by Location\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) show_states();

        else if (choice == 2) {
            printf("Enter State: ");
            get_input(input, sizeof(input));
            State *s;

            for (s = state_head; s; s = s->next)
                if (!strcmp(s->name, input))
                    show_interests(s);
        }

        else if (choice == 3) {
            printf("Enter State: ");
            get_input(input, sizeof(input));
            State *s;

            for (s = state_head; s; s = s->next)
                if (!strcmp(s->name, input)) {
                    printf("\nEnter Interest: ");
                    get_input(in, sizeof(in));
                    Interest *i;
                    for (i = s->interests; i; i = i->next)
                        if (!strcmp(i->name, in))
                            show_places(i);
                }
        }

        else if (choice == 4) show_trending();

        else if (choice == 5) {
            printf("Enter Location: ");
            get_input(input, sizeof(input));
            search_by_location(input);
        }

        else if (choice == 6) {
            printf("Goodbye!\n");
            break;
        }

        else printf("Invalid choice.\n");
    }

    return 0;
}
