// Always include the necessary header files.
// Including SFGUI/Widgets.hpp includes everything
// you can possibly need automatically.
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

int main() {

	const sf::Vector2f NOTE_WINDOW_SCR_REQ = {900.0f, 480.0f};
	const sf::Vector2f NOTE_WINDOW_POS = {330.0f, 125.0f};
	const sf::Vector2f NOTE_WINDOW_REQ = {900.0f, 500.0f};
	const sf::Vector2f ENTRY_REQ = {700.0f, 25.0f};
	const int BUTTON_FONT_SIZE = 24;
	const sf::Vector2f START_CORD = {250.0f, 650.0f};
	const sf::Vector2f CANCEL_CORD = {400.0f, 650.0f};
	const sf::Vector2f LOGO_CORD = {450.0f, .0f};
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

	auto table = sfg::Table::Create();

	sf::Image notes;
	auto note_image = sfg::Image::Create();

	if( notes.loadFromFile( "E:/Programs/Qt/Projects/sfml/images/cat.jpg" ) ) {
			note_image->SetImage( notes );
	}

	sf::Image logo;
	auto logo_image = sfg::Image::Create();

	if( logo.loadFromFile( "E:/Programs/Qt/Projects/sfml/images/mipt_logo.png" ) ) {
			logo_image->SetImage( logo );
	}

	auto note_window_scr_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	note_window_scr_box->Pack(note_image, true, true);

	auto note_window_scr = sfg::ScrolledWindow::Create();
	note_window_scr->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_NEVER | sfg::ScrolledWindow::VERTICAL_AUTOMATIC );
	note_window_scr->SetRequisition(NOTE_WINDOW_SCR_REQ);
	note_window_scr->AddWithViewport(note_window_scr_box);

	auto note_window_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	note_window_box->Pack(note_window_scr, false, true);

	auto note_window = sfg::Window::Create();
	note_window->SetTitle("Note Display");
	note_window->SetPosition(NOTE_WINDOW_POS);
	note_window->SetRequisition(NOTE_WINDOW_REQ);

	note_window->Add(note_window_box);

	auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

	auto label = sfg::Label::Create("  Please type your file path:");

	auto entry = sfg::Entry::Create("File path...");
	entry->SetRequisition(ENTRY_REQ);

	auto start_button = sfg::Button::Create( " START " );
	auto cancel_button = sfg::Button::Create( " CANCEL " );
	desktop.SetProperty("Button", "FontSize", BUTTON_FONT_SIZE);


	auto fixed = sfg::Fixed::Create();
	fixed->Put(start_button, START_CORD);
	fixed->Put(cancel_button, CANCEL_CORD);
	fixed->Put(logo_image, LOGO_CORD);

	table->Attach(entry, sf::Rect<sf::Uint32>(1, 3, 1, 1), sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f(.0f, .0f));
	table->Attach(label, sf::Rect<sf::Uint32>(1, 2, 1, 1), sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f(.0f, .0f));
	table->Attach(fixed, sf::Rect<sf::Uint32>(2, 1, 1, 40), sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f(.0f, .0f));

	box->Pack(table);


	window->Add(box);
	window->SetRequisition(WINDOW_SIZE);

	desktop.Add(note_window);
	desktop.Add(window);
	desktop.BringToFront(note_window);

	sf::Clock clock;
	// Start the game loop
	while ( app_window.isOpen() ) {
		// Process events
		sf::Event event;

		while ( app_window.pollEvent( event ) ) {
			// Handle events
			window->HandleEvent( event );
			note_window->HandleEvent( event );

			// Close window : exit
			if ( event.type == sf::Event::Closed ) {
				app_window.close();
			}
		}

		// Update the GUI, note that you shouldn't normally
		// pass 0 seconds to the update method.
		float dt = clock.restart().asSeconds();
		desktop.Update(dt);
		note_window->Update(dt);
		app_window.clear();
		sfgui.Display( app_window );
		app_window.display();
	}

	return EXIT_SUCCESS;
}
