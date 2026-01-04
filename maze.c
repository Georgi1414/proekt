#include <stdio.h>

/*
  Проект на Георги Христов(141425104) на тема "multy level puzzel game designing a puzzle game with multiple difficulty levels".
  P - играч
  # - стена
  E - изход
  Контроли: W (нагоре), S (надолу), A (наляво), D (надясно), Q (излизане от нивото).

*/

static const char *level_easy[] = {
  "#############",
  "#P   #     E#",
  "# #### ######",
  "#    #      #",
  "#### ###### #",
  "#           #",
  "#############",
};

static const char *level_medium[] = {
  "#####################",
  "#P    #             #",
  "#### #### ### ## ####",
  "#    #    #    #    #",
  "## ##### ##### ######",
  "#    #      #      E#",
  "# ## ###### # #######",
  "#  #    #   #       #",
  "## ######## ###### ##",
  "#             #     #",
  "#####################",
};

static const char *level_hard[] = {
  "#############################",
  "#P  #             #      #  #",
  "# ####### ####### # #### # ##",
  "#    #      #         #     #",
  "#### ###### # ###### ###### #",
  "#      #    #      #      # #",
  "# #### # #### #### ###### ###",
  "# #    #    #    #      #   #",
  "# # #### #### ## ######## ###",
  "# #        #  #        #  # #",
  "# ######## ## ######## #### #",
  "#        #    #            E#",
  "#############################",
};

/* списък с ниваата */
static const char **maps[] = { level_easy, level_medium, level_hard };

int rows[] = {7,11,13};
int cols[] = {13,21,29};

static char grid[20*32];
static int R,C,pr,pc;

static const char *level_name(int idx){
  if(idx == 0){
    return "Easy";
  }
  else if(idx == 1){
    return "Medium";
  }
  else{
    return "Hard";
  }
}

/* зареждане на дадено ниво */
void load(int idx){
  R = rows[idx];
  C = cols[idx];
  const char **m = maps[idx];
  int r, c;
  for(r=0; r<R; r++){
    for(c=0; c<C; c++){
      char ch = m[r][c];
      if(ch == 'P'){
        pr = r;
        pc = c;
        grid[r*C+c] = ' ';
      }
      else{
        grid[r*C+c] = ch;
      }
    }
  }
}
/*легенда на лабирнтът */
void draw(const char *name, int moves){
  /* изчиства екрана използваики ANSI escape кодове*/
  printf("\033[2J\033[H");
  printf("Level: %s    Moves: %d\n", name, moves);
  printf("Instructions: type W/A/S/D and press Enter to move, (Q to quit).\n\n");
  int r, c;
  for(r=0; r<R; r++){
    for(c=0; c<C; c++){
      if(r == pr && c == pc){
        putchar('P');
        
      }
      else{
        putchar(grid[r*C+c]);
      }
    }
    putchar('\n');
  }
}

/* проверяваме за грешки при движение */
int can(int dr, int dc){
  int nr = pr + dr;
  int nc = pc + dc;
  if(nr < 0){
    return 0;
  }
  if(nr >= R){
    return 0;
  }
  if(nc < 0){
    return 0;
  }
  if(nc >= C){
    return 0;
  }
  char t = grid[nr*C + nc];
  if(t == '#'){
    return 0;
  }
  if(t == 'E'){
    return 2;
  }
  pr = nr;
  pc = nc;
  return 1;
}

/* правим самото движение */
int process_char(int ch, int *moves){
  char c = (char) ch;
  if(c >= 'a' && c <= 'z'){
    c = c - 32;
  }
  if(c == '\n'){
    return 0;
  }
  if(c == '\r'){
    return 0;
  }
  if(c == ' '){
    return 0;
  }
  if(c == '\t'){
    return 0;
  }
  if(c == 'Q'){
    return -1;
  }
  int res = 0;
  if(c == 'W'){
    res = can(-1, 0);
  }
  else if(c == 'S'){
    res = can(1, 0);
  }
  else if(c == 'A'){
    res = can(0, -1);
  }
  else if(c == 'D'){
    res = can(0, 1);
  }
  if(res == 2){
    (*moves)++;
    return 2;
  }
  if(res == 1){
    (*moves)++;
    return 1;
  }
  return 0;
}

/* главен цикъл на играта */
int game_loop(int idx){
  int moves = 0;
  load(idx);
  draw(level_name(idx), moves);
  int ch;
  while((ch = getchar()) != EOF){
    int r = process_char(ch, &moves);
    if(r < 0){
      puts("Quit...");
      return 0;
    }
    if(r == 2){
      draw(level_name(idx), moves);
      printf("\nCongratulations - you found the exit in %d moves!\n", moves);
      printf("Press Enter to exit...");
      int c = getchar();
      if(c == '\n'){
        c = getchar();
        while(c != EOF){
          if(c == '\n'){
            break;
          }
          c = getchar();
        }
      }
      else{
        while(c != EOF){
          if(c == '\n'){
            break;
          }
          c = getchar();
        }
      }
      return 0;
    }
    draw(level_name(idx), moves);
  }
  return 0;
}

int main(void) {
  puts("=== Terminal Maze (C) ===");
  int current = -1;

  /* избор на ниво */
  printf("Choose difficulty (1 - Easy, 2 - Medium, 3 - Hard) or Q to quit: ");
  int ch = getchar();
  if(ch == EOF){
    return 0;
  }
  while(ch != EOF){
    if(ch == '\n'){
      ch = getchar();
      continue;
    }
    if(ch == ' '){
      ch = getchar();
      continue;
    }
    if(ch == '\r'){
      ch = getchar();
      continue;
    }
    if(ch == '\t'){
      ch = getchar();
      continue;
    }
    break;
  }
  if(ch == EOF){
    return 0;
  }
  if(ch == 'Q' || ch == 'q'){
    return 0;
  }
  if(ch == '1'){
    current = 0;
  }
  else if(ch == '2'){
    current = 1;
  }
  else if(ch == '3'){
    current = 2;
  }
  else{
    puts("Invalid choice, exiting.");
    return 0;
  }
  while(ch != EOF){
    if(ch == '\n'){
      break;
    }
    ch = getchar();
  }

  /* стартира играта */
  game_loop(current);
  return 0;
}
