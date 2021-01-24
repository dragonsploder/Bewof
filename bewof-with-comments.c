#define F(v,h)for(v=0;v<h;v++) // For loop short hand
#define I F(y,22)F(x,80) // Two for loops to cycle through every tile 
#define U M[y][x] // Short hand for int at map location y, x
#define R rand()% // Short hand for rand
#define Z U<106&U>96 // Short hand to check if a tile is a monster or not


/* Variables:
 * M: The game map
 * y, x: For loop variables
 * Y, X: Players current location
 * c: Player input
 * t: Turn
 * m: How many monsters did we start with (used for scoring)
 * h: Player Hp
 * r, i: Misc variables
 * l: Turn when teleport was last used
 * T: Teleport toggle
 * W: 2d3 weapon toggle
 * H: Sneak toggle
 */
M[22][80],y,x,Y,X,c,t=0,m=0,h=20,r,i,l,T,W,H;

// Function which checks if game is over (stores value in i), and if so, prints out the score
S(){
    r=0;
    I r+=Z?1:0; // For every tile, if there is a monster, increment r
    i=
        r? // If r greater then 0, there is at least one monster
            1 // "return" true
        : // Else there are no monsters
            printf("Score:%i\n",(m*10) - t + (h*10)), // Print score
            0 // "return" false
        ;
}

// Function which teleports the player
L(){
    l=t; // Reset last teleport count (l)
    while(U!=46){ // While an empty tile is not found
        // Pick a random tile
        x=R 80; 
        y=R 22;
    };
    // Set new player location
    U=64; // Move player
    M[Y][X]=46; // Empty old player tile
    // Set player location
    Y=y;
    X=x;
}

// main function
// a: Count of inputs
// A: List of inputs
main(int a,char*A[]){
    F(i,a){ // For every input
        r=*A[i]; // Set r to input
        r==84? // If arg is 'T'
            T=1 // Set Teleport flag
        :
            r==72? // Else if arg is 'H'
                H=1 // Set Sneak flag
            :
                r==87? // Else if arg is 'W'
                    W=1 // Set Weapon flag
                :
                    0 // Else do nothing
                ;
    };

    srand(time(0)); // Seed random
  
    initscr(); // Init ncurses

    I U = 35; // Fill map with walls (#)

    // Set y, x to the middle of the map
    y=11;
    x=40;

    // Simple drunk walk algorithem
    F(i,2000){ // 2000 steps creates a nice size cave
        U= // Set current tile
            R 20? // random number 0-19
                46 // 19/20 chance for empty tile (.)
            :   
                97+R 8, // 1/20 chance for monster (a-g)
                m++; // Increase monster count
        r=R 4; // Pick random direction
        !r&y>1? // If r==0 and not out-of-bounds
            y-- // Move current location up
        :
            r==1&y<20? // If r==1 and not out-of-bounds
                y++ // Move current location down
            :
                r==2&x>1? // If r==2 and not out-of-bounds
                    x-- // Move current location left
                :
                    x<78? // If r==3 and not out-of-bounds
                        x++ // Move current location right
                    :
                        0 // Else do nothing
                    ;
    }

    // Set player location at last point of drunk walk
    U=64; // Set tile to at-sign (@)
    Y=y; // Set player Y
    X=x; // Set player X
    
    // Main game loop
    do {
        I mvaddch(y,x,U); // Print screen
        r= // r is used as a toggle for if the player can teleport
            t>l+19; // current turn > last teleport turn + 19
        mvprintw(22,0,"Hp:%i %c", h, r?94:32/*add ^ if can teleport*/); // Print status message
        c=getch(); // Get user input
        t++; // increment turn counter

        l+=!T; // Increment teleport counter if necessary

        c==94&r? // If user input (^) and can teleport
            L() // Call teleport function
        :
            0 // Else do nothing
        ;

        // Set current tile to player location
        y=Y; 
        x=X;
        
        // Player movement
        //  This loop has two stages (noted by r), a move current location stage, and a attack stage
        //      If the current tile is empty, the player is moved to the current tile
        //      If there is a monster, the loop runs again in the attack stage
        F(r,2){ 
            c==119? // If user input is 'w' (True attack)
                
                r? // If attack stage
                    W? // If player has weapons toggle
                        U-=R 3+R 3 // Subtract from monster 2d3
                    : // If no weapon 
                        U--, // Subtract 1 from monster
                        U<97? // If monster is (a)
                            U=46 // Set tile to floor (.)
                        :
                            0 // Else do nothing
                :
                    y-- // Move current tile up
                    
            :
            c==115? // If user input is 's' (Push)
                r? // If attack stage
                    M[y+1][x]!=35? // If tile monster will be pushed into is not a wall
                        M[y+1][x]=U-9, // Move the monster down one (set move flag)
                        U=46 // Set old tile to empty (.)
                    :
                        0 // Else do nothing
                :
                    y++ // Move current tile down

            :
            c==100? // If user input is 'd' (Swap)
                r? // If attack stage
                    M[Y][X]=U-9, // Place monster were player is (set move flag)
                    U=64, // Place player were monster was
                    X++ // Update player location
                :
                    x++ // Move current tile right

            :
            c==97?
                r? // If attack stage
                    M[Y-2][X]!=35&Y>1? // If two space above player is not wall (and not out of bounds)
                        M[Y-2][X]=U-9, // Move monster (set move flag)
                        U=46 // set old tile to empty (.)
                    :
                        0 // Else do nothing
                :
                    x-- // Move current tile left
            :
                0 // Else skip turn
            ;


            r? // If attack stage
                0 // Do nothing
            :
                U==46? // If current location is empty (.)
                    M[Y][X]=46, // Set player location to be empty 
                    U=64, // Set current location to player
                    //Update current player location
                    X=x, 
                    Y=y
                :
                    0 // Else do nothing
                ;
            
            Z? // If current tile is monster
                0 // Do nothing
            :
                r++ // Increment r so we don't enter the attack stage
            ;
        }

        // This next block of code handles monster ai
        I { // For every tile
            Z& // If tile is a monster
              (H& // If sneak flage is toggled
                 (X-x>5|x-X>5|Y-y>5|y-Y>5)? // And id monster is NOT in 5 tile range
                    0 // return false
                :
                    1 // Else sneak is off or monster is within 5 tile range, either way, return true
                )?

                // Variables i and r will be used to represent the tile the monster wants to move to
                i=y, 
                r=x,
                X==x? // If on the same vertical axis as the player
                    Y>y? // If above the player
                        i++ // Move down one
                    :
                        i-- // Move up one
                :
                    Y==y? // If on the same horizontal axis
                        X>x? // If to the left of the player
                            r++ // Move right one
                        :
                            r-- // Move left one
                    :  
                        M[i][r]==46? // If next tile is empty (.)
                            M[i][r]=U-9, // Apply move flage
                            U=46 // Empty (.) old tile 
                        :
                            M[i][r]==64? // If the next tile is the player (@)
                                h-- // Remove one hp
                            :
                                0
            :
                0
            ;
        }  
        I -9+Z-9?U+=9:0; // For every tile, undo move flag
        S(); // Is game over (store in i)
        // Loop while:
    } while(c!=81 // Command != Q
        &h>0 // Hp is greater then 0
        &i); // Game not over

    // End ncurses 
    endwin();
}