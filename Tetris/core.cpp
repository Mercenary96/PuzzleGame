

#include "core.h"


std::vector<SDL_Rect> Game::number_clips;


void Game::InitWinAndRen()
{

	win.reset(SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, 50,
		480, 800, SDL_WINDOW_SHOWN));


	ren.reset(SDL_CreateRenderer(win.get(), -1, SDL_RENDERER_ACCELERATED
		| SDL_RENDERER_PRESENTVSYNC));

	if (win == nullptr || ren == nullptr)
	{

		std::cout << "Problem initialising window and/or renderer.\n";
		
	}

}


Game::Game() : win(nullptr, SDL_DestroyWindow),
	ren(nullptr, SDL_DestroyRenderer),
	numbers(nullptr, SDL_DestroyTexture)
{

	InitWinAndRen();

	numbers.reset(IMG_LoadTexture(ren.get(), "Resources\\numbers.png"));
	
	for (int i = 0; i < 320; i += 32)
	{
		SDL_Rect temp;
		temp.x = i;
		temp.y = 0;
		temp.w = 32;
		temp.h = 32;

		number_clips.push_back(temp);
	}

	score = 00000000;

	gen_block = true;

}


bool Game::EventLoop()
{

	SDL_Event e;
	while (SDL_PollEvent(&e))
	{

		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE
			|| e.type == SDL_QUIT)
		{

			return true;

		}

	}

	return false;

}


void Game::DrawScore()
{

	std::string score_string;

	std::stringstream my_stream;

	my_stream << score;
	my_stream >> score_string;

	for (unsigned int i = 0; i < score_string.size(); ++i)
	{

		PrintScore(score_string[i], number_clips[i]);

	}



}


void Game::PrintScore(char number, SDL_Rect location)
{
	
	std::stringstream convertor;

	convertor << number;

	int my_num;

	convertor >> my_num;

	SDL_RenderCopy(ren.get(), numbers.get(), &number_clips[my_num], &location);
	
}


void Game::CheckForGenBlock()
{

	// checks if a new block should be generated
	if (gen_block)
	{

		controller.GenerateRandomBlock();

		gen_block = false;

	}

}


void Game::DrawAndCheckBoardAddition(float frame_time)
{

	// Draws the block at it's current location - 
	// returns true if the block has collided with another.
	if (controller.DrawBlock(ren.get(), game_board.board_squares, frame_time))
	{

		game_board.AddToBoard(controller.GetCurrentBlock(), score);

		gen_block = true;

	}

}


void Game::Run()
{

	controller = BlockControl(ren.get());

	game_board = Board(ren.get());

	float frame_time = 0.0;

	while (!EventLoop())
	{

		frame_timer.StartTimer();

		SDL_SetRenderDrawColor(ren.get(), 127, 127, 127, 255);
		
		SDL_RenderClear(ren.get());
		
		controller.MoveBlock(ren.get(), game_board.board_squares, frame_time);
		
		SDL_Delay(0034); // aim for 30 fps
		
		CheckForGenBlock();
		
		DrawAndCheckBoardAddition(frame_time);

		if (game_board.DrawBoardBlocks(ren.get())) break;
		
		DrawScore();
		
		SDL_RenderPresent(ren.get());

		frame_timer.StopTimer();

		frame_time = frame_timer.GetTimeSeconds();
		
	}

}