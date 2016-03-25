// Always include the necessary header files.
// Including SFGUI/Widgets.hpp includes everything
// you can possibly need automatically.
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

int main() {

	const sf::Vector2f NOTE_WINDOW_SCR_REQ = {800.0f, 480.0f};
	const sf::Vector2f NOTE_WINDOW_POS = {425.0f, 100.0f};
	const sf::Vector2f NOTE_WINDOW_REQ = {800.0f, 500.0f};
	const sf::Vector2f SONG_PATH_REQ = {250.0f, .0f};
	const sf::Vector2f SONG_WINDOW_SCR_REQ = {250.0f, 555.0f};
	const sf::Vector2f SONG_WINDOW_POS = {25.0f, 100.0f};
	const sf::Vector2f SONG_WINDOW_REQ = {375.0f, 575.0f};
	const sf::Vector2f ENTRY_REQ = {700.0f, 25.0f};
	const int BUTTON_FONT_SIZE = 24;
	const int LOAD_FONT_SIZE = 12;
	const sf::Vector2f START_COORD = {125.0f, 550.0f};
	const sf::Vector2f CANCEL_COORD = {350.0f, 550.0f};
	const sf::Vector2f GETIMAGE_COORD = {575.0f, 550.0f};
	const sf::Vector2f LOGO_COORD = {.0f, .0f};
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

	sf::Image notes;
	auto note_image = sfg::Image::Create();

	if( notes.loadFromFile( "../sfml/images/cat.jpg" ) ) {
			note_image->SetImage( notes );
	}

	sf::Image logo;
	auto logo_image = sfg::Image::Create();

	if( logo.loadFromFile( "../sfml/images/mipt_logo_long.png" ) ) {
			logo_image->SetImage( logo );
	}

	//Editing note window
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

	//Editing song window
	auto song_window_scr_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	auto space = sfg::Label::Create(" ");
	auto song_path = sfg::Label::Create("	C:/MyProgram/Music/MIPT/Main_Theme.wav");
	song_path->SetRequisition(SONG_PATH_REQ);
	auto load_button = sfg::Button::Create("LOAD");
	load_button->SetId("load_button");
	//font size set below after setting font size for every button to be BUTTON_FONT_SIZE
	auto space_ = sfg::Label::Create(" ");
	auto song_path_ = sfg::Label::Create("	C:/MyProgram/Music/BATAREYKA.wav");
	song_path_->SetRequisition(SONG_PATH_REQ);
	auto load_button_ = sfg::Button::Create("LOAD");
	load_button_->SetId("load_button");
	//font size set below after setting font size for every button to be BUTTON_FONT_SIZE

	auto song_table = sfg::Table::Create();
	song_table->Attach(space, sf::Rect<sf::Uint32>(1, 1, 2, 1), sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f(.0f, .0f));
	song_table->Attach(song_path, sf::Rect<sf::Uint32>(1, 2, 1, 1), sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(10.0f, 5.0f));
	song_table->Attach(load_button, sf::Rect<sf::Uint32>(2, 2, 1, 1), sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f(.0f, .0f));

	song_table->Attach(space_, sf::Rect<sf::Uint32>(1, 3, 2, 1), sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f(.0f, .0f));
	song_table->Attach(song_path_, sf::Rect<sf::Uint32>(1, 4, 1, 1), sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(15.0f, 5.0f));
	song_table->Attach(load_button_, sf::Rect<sf::Uint32>(2, 4, 1, 1), sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f(.0f, .0f));

	song_window_scr_box->Pack(song_table);

	auto song_window_scr = sfg::ScrolledWindow::Create();
	song_window_scr->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC );
	song_window_scr->SetRequisition(SONG_WINDOW_SCR_REQ);
	song_window_scr->AddWithViewport(song_window_scr_box);

	auto song_window_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	song_window_box->Pack(song_window_scr, false, true);

	auto song_window = sfg::Window::Create();
	song_window->SetTitle("Song Display");
	song_window->SetPosition(SONG_WINDOW_POS);
	song_window->SetRequisition(SONG_WINDOW_REQ);

	song_window->Add(song_window_box);

	//Editing main window
	auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

	auto label = sfg::Label::Create("  Please type your file path and press 'LOAD':");

	auto entry = sfg::Entry::Create("File path...");
	entry->SetRequisition(ENTRY_REQ);

	auto entry_load_button = sfg::Button::Create("LOAD");
	entry_load_button->SetId("load_button");

	auto start_button = sfg::Button::Create("    START    ");
	auto cancel_button = sfg::Button::Create("   CANCEL   ");
	auto getimage_button = sfg::Button::Create(" GET IMAGE ");
	desktop.SetProperty("Button", "FontSize", BUTTON_FONT_SIZE);
	desktop.SetProperty("Button#load_button", "FontSize", LOAD_FONT_SIZE);


	auto fixed_b = sfg::Fixed::Create();
	fixed_b->Put(start_button, START_COORD);
	fixed_b->Put(cancel_button, CANCEL_COORD);
	fixed_b->Put(getimage_button, GETIMAGE_COORD);

	auto fixed_img = sfg::Fixed::Create();
	fixed_img->Put(logo_image, LOGO_COORD);

	//Creating table for displaying elements on certain places
	auto table = sfg::Table::Create();
	table->Attach(entry, sf::Rect<sf::Uint32>(1, 3, 2, 1), sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f(.0f, .0f));
	table->Attach(label, sf::Rect<sf::Uint32>(1, 2, 2, 1), sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f(.0f, .0f));
	table->Attach(entry_load_button, sf::Rect<sf::Uint32>(3, 3, 1, 1), sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(.0f, .0f));
	table->Attach(fixed_img, sf::Rect<sf::Uint32>(4, 1, 1, 10), sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f(.0f, .0f));
	table->Attach(fixed_b, sf::Rect<sf::Uint32>(2, 11, 3, 30), sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f(.0f, .0f));

	box->Pack(table);

	window->Add(box);
	window->SetRequisition(WINDOW_SIZE);

	//Displaying everything
	desktop.Add(note_window);
	desktop.Add(song_window);
	desktop.Add(window);
	desktop.BringToFront(note_window);
	desktop.BringToFront(song_window);

	sf::Clock clock;
	// Start the game loop
	while ( app_window.isOpen() ) {
		// Process events
		sf::Event event;

		while ( app_window.pollEvent(event)) {
			// Handle events (setting windows for being active)
			window->HandleEvent(event);
			note_window->HandleEvent(event);
			song_window->HandleEvent(event);

			// Close window : exit
			if ( event.type == sf::Event::Closed ) {
				app_window.close();
			}
		}

		// Update the GUI, note that you shouldn't normally
		// pass 0 seconds to the update method.
		float dt = clock.restart().asSeconds();
		desktop.Update(dt);
		//note_window->Update(dt);
		app_window.clear();
		sfgui.Display(app_window);
		app_window.display();
	}

	return EXIT_SUCCESS;
}
