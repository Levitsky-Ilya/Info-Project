// Always include the necessary header files.
// Including SFGUI/Widgets.hpp includes everything
// you can possibly need automatically.
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

int main() {

	const sf::Vector2f noteWindowScrolled_REQ = {800.0f, 480.0f};
	const sf::Vector2f noteWindow_POS = {425.0f, 100.0f};
	const sf::Vector2f noteWindow_REQ = {800.0f, 500.0f};
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
	sf::RenderWindow appWindow(BASE_SIZE, "My program",
			sf::Style::Titlebar | sf::Style::Close);

	// We have to do this because we don't use SFML to draw.
	appWindow.resetGLStates();

	// Create an SFGUI. This is required before doing anything with SFGUI.
	sfg::SFGUI sfgui;

	sfg::Desktop desktop;

	// Create our main SFGUI window
	auto window = sfg::Window::Create(sf::Style::Resize);

	sf::Image notes;
	auto noteImage = sfg::Image::Create();

	if( notes.loadFromFile( "../sfml/images/cat.jpg" ) ) {
			noteImage->SetImage( notes );
	}

	sf::Image logo;
	auto logoImage = sfg::Image::Create();

	if( logo.loadFromFile( "../sfml/images/mipt_logo_long.png" ) ) {
			logoImage->SetImage( logo );
	}

	//Editing note window
	auto noteWindowScrolledBox =
			sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	noteWindowScrolledBox->Pack(noteImage, true, true);

	auto noteWindowScrolled = sfg::ScrolledWindow::Create();
	noteWindowScrolled->
			SetScrollbarPolicy(sfg::ScrolledWindow::HORIZONTAL_NEVER
			| sfg::ScrolledWindow::VERTICAL_AUTOMATIC);
	noteWindowScrolled->SetRequisition(noteWindowScrolled_REQ);
	noteWindowScrolled->AddWithViewport(noteWindowScrolledBox);

	auto noteWindowBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	noteWindowBox->Pack(noteWindowScrolled, false, true);

	auto noteWindow = sfg::Window::Create();
	noteWindow->SetTitle("Note Display");
	noteWindow->SetPosition(noteWindow_POS);
	noteWindow->SetRequisition(noteWindow_REQ);

	noteWindow->Add(noteWindowBox);

	//Editing song window
	auto songWindowScrolledBox =
			sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	auto space = sfg::Label::Create(" ");
	auto songPath =
			sfg::Label::Create("	C:/MyProgram/Music/MIPT/Main_Theme.wav");
	songPath->SetRequisition(SONG_PATH_REQ);
	auto loadButton = sfg::Button::Create("LOAD");
	loadButton->SetId("loadButton");
	//font size set below after setting font size
	//for every button to be BUTTON_FONT_SIZE
	auto space_ = sfg::Label::Create(" ");
	auto songPath_ = sfg::Label::Create("	C:/MyProgram/Music/BATAREYKA.wav");
	songPath_->SetRequisition(SONG_PATH_REQ);
	auto loadButton_ = sfg::Button::Create("LOAD");
	loadButton_->SetId("loadButton");
	//font size set below after setting font size
	//for every button to be BUTTON_FONT_SIZE

	auto song_table = sfg::Table::Create();
	song_table->Attach(space,
					   sf::Rect<sf::Uint32>(1, 1, 2, 1),
					   sfg::Table::EXPAND,
					   sfg::Table::EXPAND,
					   sf::Vector2f(.0f, .0f));
	song_table->Attach(songPath,
					   sf::Rect<sf::Uint32>(1, 2, 1, 1),
					   sfg::Table::EXPAND,
					   sfg::Table::FILL,
					   sf::Vector2f(10.0f, 5.0f));
	song_table->Attach(loadButton,
					   sf::Rect<sf::Uint32>(2, 2, 1, 1),
					   sfg::Table::EXPAND,
					   sfg::Table::EXPAND,
					   sf::Vector2f(.0f, .0f));

	song_table->Attach(space_,
					   sf::Rect<sf::Uint32>(1, 3, 2, 1),
					   sfg::Table::EXPAND,
					   sfg::Table::EXPAND,
					   sf::Vector2f(.0f, .0f));
	song_table->Attach(songPath_,
					   sf::Rect<sf::Uint32>(1, 4, 1, 1),
					   sfg::Table::EXPAND,
					   sfg::Table::FILL,
					   sf::Vector2f(15.0f, 5.0f));
	song_table->Attach(loadButton_,
					   sf::Rect<sf::Uint32>(2, 4, 1, 1),
					   sfg::Table::EXPAND,
					   sfg::Table::EXPAND,
					   sf::Vector2f(.0f, .0f));

	songWindowScrolledBox->Pack(song_table);

	auto songWindowScrolled = sfg::ScrolledWindow::Create();
	songWindowScrolled->
			SetScrollbarPolicy(sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC
			| sfg::ScrolledWindow::VERTICAL_AUTOMATIC);
	songWindowScrolled->SetRequisition(SONG_WINDOW_SCR_REQ);
	songWindowScrolled->AddWithViewport(songWindowScrolledBox);

	auto songWindowBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	songWindowBox->Pack(songWindowScrolled, false, true);

	auto songWindow = sfg::Window::Create();
	songWindow->SetTitle("Song Display");
	songWindow->SetPosition(SONG_WINDOW_POS);
	songWindow->SetRequisition(SONG_WINDOW_REQ);

	songWindow->Add(songWindowBox);

	//Editing main window
	auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

	auto label =
			sfg::Label::Create("  Please type your file path and press 'LOAD':");

	auto entry = sfg::Entry::Create("File path...");
	entry->SetRequisition(ENTRY_REQ);

	auto entryLoadButton = sfg::Button::Create("LOAD");
	entryLoadButton->SetId("loadButton");

	auto startButton = sfg::Button::Create("    START    ");
	auto cancelButton = sfg::Button::Create("   CANCEL   ");
	auto getimageButton = sfg::Button::Create(" GET IMAGE ");
	desktop.SetProperty("Button", "FontSize", BUTTON_FONT_SIZE);
	desktop.SetProperty("Button#loadButton", "FontSize", LOAD_FONT_SIZE);


	auto fixed_b = sfg::Fixed::Create();
	fixed_b->Put(startButton, START_COORD);
	fixed_b->Put(cancelButton, CANCEL_COORD);
	fixed_b->Put(getimageButton, GETIMAGE_COORD);

	auto fixedImg = sfg::Fixed::Create();
	fixedImg->Put(logoImage, LOGO_COORD);

	//Creating table for displaying elements on certain places
	auto table = sfg::Table::Create();
	table->Attach(entry,
				  sf::Rect<sf::Uint32>(1, 3, 2, 1),
				  sfg::Table::EXPAND,
				  sfg::Table::EXPAND,
				  sf::Vector2f(.0f, .0f));
	table->Attach(label,
				  sf::Rect<sf::Uint32>(1, 2, 2, 1),
				  sfg::Table::EXPAND,
				  sfg::Table::EXPAND,
				  sf::Vector2f(.0f, .0f));
	table->Attach(entryLoadButton,
				  sf::Rect<sf::Uint32>(3, 3, 1, 1),
				  sfg::Table::EXPAND,
				  sfg::Table::FILL,
				  sf::Vector2f(.0f, .0f));
	table->Attach(fixedImg,
				  sf::Rect<sf::Uint32>(4, 1, 1, 10),
				  sfg::Table::EXPAND,
				  sfg::Table::EXPAND,
				  sf::Vector2f(.0f, .0f));
	table->Attach(fixed_b,
				  sf::Rect<sf::Uint32>(2, 11, 3, 30),
				  sfg::Table::EXPAND,
				  sfg::Table::EXPAND,
				  sf::Vector2f(.0f, .0f));

	box->Pack(table);

	window->Add(box);
	window->SetRequisition(WINDOW_SIZE);

	//Displaying everything
	desktop.Add(noteWindow);
	desktop.Add(songWindow);
	desktop.Add(window);
	desktop.BringToFront(noteWindow);
	desktop.BringToFront(songWindow);

	sf::Clock clock;
	// Start the game loop
	while ( appWindow.isOpen() ) {
		// Process events
		sf::Event event;

		while ( appWindow.pollEvent(event)) {
			// Handle events (setting windows for being active)
			window->HandleEvent(event);
			noteWindow->HandleEvent(event);
			songWindow->HandleEvent(event);

			// Close window : exit
			if ( event.type == sf::Event::Closed ) {
				appWindow.close();
			}
		}

		// Update the GUI, note that you shouldn't normally
		// pass 0 seconds to the update method.
		float dt = clock.restart().asSeconds();
		desktop.Update(dt);
		//noteWindow->Update(dt);
		appWindow.clear();
		sfgui.Display(appWindow);
		appWindow.display();
	}

	return EXIT_SUCCESS;
}
