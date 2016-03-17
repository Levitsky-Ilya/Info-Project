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

	//sfg::Desktop desktop;

	// Create our main SFGUI window
	auto window = sfg::Window::Create(sf::Style::Resize);

	sf::Image logo;
	auto image = sfg::Image::Create();

	if( logo.loadFromFile( "E:/Programs/Qt/Projects/sfml/images/4226899.png" ) ) {
			image->SetImage( logo );
	}

	auto frame_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 20.f);
	auto frame = sfg::Frame::Create("Notes Display");
	frame_box->Pack(image, true, true);
	frame->Add(frame_box);

	auto separator = sfg::Separator::Create(sfg::Separator::Orientation::HORIZONTAL);
	frame_box->Pack(separator, true, true);


	auto box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 20.f );

	auto start_button = sfg::Button::Create( "START" );
	auto cancel_button = sfg::Button::Create( "CANCEL" );

	auto fixed = sfg::Fixed::Create();
	fixed->Put( start_button, sf::Vector2f( 1105.f, 275.f ) );
	fixed->Put( cancel_button, sf::Vector2f( 1205.f, 275.f ) );

	box->Pack(frame, true, true);
	box->Pack(fixed, false, true);


	window->Add( box );


	//desktop.Add(window);

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
		app_window.clear();
		sfgui.Display( app_window );
		app_window.display();
	}

	return EXIT_SUCCESS;
}
