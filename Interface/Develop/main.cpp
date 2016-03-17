// Always include the necessary header files.
// Including SFGUI/Widgets.hpp includes everything
// you can possibly need automatically.
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

int main() {
	// Create the main SFML window
	sf::RenderWindow app_window( sf::VideoMode( 1280, 720 ), "My program", sf::Style::Titlebar | sf::Style::Close );

	// We have to do this because we don't use SFML to draw.
	app_window.resetGLStates();

	// Create an SFGUI. This is required before doing anything with SFGUI.
	sfg::SFGUI sfgui;

	// Create our main SFGUI window
	auto window = sfg::Window::Create(sf::Style::Resize);

	// Create a box with 20 pixels spacing.
	auto box = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL, 20.f );

	// Create some buttons.
	auto start_button = sfg::Button::Create( "START" );
	auto cancel_button = sfg::Button::Create( "CANCEL" );

	auto fixed = sfg::Fixed::Create();
	fixed->Put( start_button, sf::Vector2f( 1105.f, 675.f ) );
	fixed->Put( cancel_button, sf::Vector2f( 1205.f, 675.f ) );

	box->Pack(fixed, false, true);

	// Add the box to the window.
	window->Add( box );

	// Start the game loop
	while ( app_window.isOpen() ) {
		// Process events
		sf::Event event;

		while ( app_window.pollEvent( event ) ) {
			// Handle events
			window->HandleEvent( event );

			// Close window : exit
			if ( event.type == sf::Event::Closed ) {
				app_window.close();
			}
		}

		// Update the GUI, note that you shouldn't normally
		// pass 0 seconds to the update method.
		window->Update( 0.f );

		// Clear screen
		app_window.clear();

		// Draw the GUI
		sfgui.Display( app_window );

		// Update the window
		app_window.display();
	}

	return EXIT_SUCCESS;
}
