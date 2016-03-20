// Always include the necessary header files.
// Including SFGUI/Widgets.hpp includes everything
// you can possibly need automatically.
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

int main() {

	const sf::Vector2f NOTE_WINDOW_POS = {330.0f, 75.0f};
	const sf::Vector2f NOTE_WINDOW_REQ = {900.0f, 500.0f};
	const int BUTTON_FONT_SIZE = 24;
	const sf::Vector2f START_CORD = {900.0f, 625.0f};
	const sf::Vector2f CANCEL_CORD = {1050.0f, 625.0f};
	const sf::Vector2f WINDOW_SIZE = {1280.0f, 720.f};
	const sf::VideoMode BASE_SIZE = {1280, 720};



	// Create the main SFML window
	sf::RenderWindow app_window( BASE_SIZE, "My program", sf::Style::Titlebar | sf::Style::Close );

	// We have to do this because we don't use SFML to draw.
	app_window.resetGLStates();

	// Create an SFGUI. This is required before doing anything with SFGUI.
	sfg::SFGUI sfgui;

	sfg::Desktop desktop;

	// Create our main SFGUI window
	auto window = sfg::Window::Create(sf::Style::Resize);

	sf::Image logo;
	auto image = sfg::Image::Create();

	if( logo.loadFromFile( "E:/Programs/Qt/Projects/sfml/images/4226899.png" ) ) {
			image->SetImage( logo );
	}


	auto note_window = sfg::Window::Create();
	note_window->SetTitle("Note Display");
	note_window->SetPosition(NOTE_WINDOW_POS);
	note_window->SetRequisition(NOTE_WINDOW_REQ);


	note_window->Add(image);


	auto box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL );

	auto start_button = sfg::Button::Create( "START" );
	auto cancel_button = sfg::Button::Create( "CANCEL" );
	desktop.SetProperty("Button", "FontSize", BUTTON_FONT_SIZE);

	auto fixed = sfg::Fixed::Create();
	fixed->Put(start_button, sf::Vector2f(START_CORD));
	fixed->Put(cancel_button, sf::Vector2f(CANCEL_CORD));

	box->Pack(fixed, false, true);

	window->Add(box);
	window->SetRequisition(WINDOW_SIZE);

	desktop.Add(note_window);
	desktop.Add(window);
	desktop.BringToFront(note_window);

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
		desktop.Update(.0f);
		app_window.clear();
		sfgui.Display( app_window );
		app_window.display();
	}

	return EXIT_SUCCESS;
}
