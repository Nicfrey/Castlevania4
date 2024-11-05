#pragma once
#include "structs.h"
#include "SDL.h"
// https://BaseGameprogrammingpatterns.com/subclass-sandbox.html


class BaseGame
{
public:
	explicit BaseGame( const Window& window );
	BaseGame( const BaseGame& other ) = delete;
	BaseGame& operator=( const BaseGame& other ) = delete;
	BaseGame(BaseGame&& other) = delete;
	BaseGame& operator=(BaseGame&& other) = delete;
	virtual ~BaseGame( );

	void Run( );

	virtual void Update(float elapsedSec, bool& quit)
	{
		
	}
	virtual void Draw() const
	{
		
	}

	// Event handling
	virtual void ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
	{
		
	}
	virtual void ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
	{
		
	}
	virtual void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
	{
		
	}
	virtual void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
	{
		
	}
	virtual void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
	{
		
	}

	void SetViewPort(float width, float height)
	{
		m_Viewport.width = width;
		m_Viewport.height = height;

		m_Window.width = width;
		m_Window.height = height;

		SDL_SetWindowSize(m_pWindow, int(width), int(height));
		glViewport(0, 0, int(m_Window.width), int(m_Window.height));
	}

	const Rectf& GetViewPort() const
	{
		return m_Viewport;
	}

private:
	// DATA MEMBERS
	// The window properties
	Window m_Window;
	Rectf m_Viewport;
	// The window we render to
	SDL_Window* m_pWindow;
	// OpenGL context
	SDL_GLContext m_pContext;
	// Init info
	bool m_Initialized;
	// Prevent timing jumps when debugging
	const float m_MaxElapsedSeconds;
	
	// FUNCTIONS
	void InitializeGameEngine( );
	void CleanupGameEngine( );
};
