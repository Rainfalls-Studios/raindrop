#ifndef __RAINDROP_EVENT_HPP__
#define __RAINDROP_EVENT_HPP__

#include "common.hpp"
#include "Utils/Flags.hpp"

#include <unordered_set>
#include <queue>

/**
 * TODO
 * 
 * replace the event class by an event wrapper that stores an ID and referes to data in the manager
 * Storing references to events can lead to issues when reallocating the event pool and moving away the actual event data.
 * 
 */

namespace Raindrop{
	namespace Internal{
		class Context;
	}

	class Event{
		public:
			// based of SDL_SCANCODE keys
			enum Key{
				KEY_A = 4,
				KEY_B = 5,
				KEY_C = 6,
				KEY_D = 7,
				KEY_E = 8,
				KEY_F = 9,
				KEY_G = 10,
				KEY_H = 11,
				KEY_I = 12,
				KEY_J = 13,
				KEY_K = 14,
				KEY_L = 15,
				KEY_M = 16,
				KEY_N = 17,
				KEY_O = 18,
				KEY_P = 19,
				KEY_Q = 20,
				KEY_R = 21,
				KEY_S = 22,
				KEY_T = 23,
				KEY_U = 24,
				KEY_V = 25,
				KEY_W = 26,
				KEY_X = 27,
				KEY_Y = 28,
				KEY_Z = 29,
				KEY_1 = 30,
				KEY_2 = 31,
				KEY_3 = 32,
				KEY_4 = 33,
				KEY_5 = 34,
				KEY_6 = 35,
				KEY_7 = 36,
				KEY_8 = 37,
				KEY_9 = 38,
				KEY_0 = 39,
				KEY_RETURN = 40,
				KEY_ESCAPE = 41,
				KEY_BACKSPACE = 42,
				KEY_TAB = 43,
				KEY_SPACE = 44,
				KEY_MINUS = 45,
				KEY_EQUALS = 46,
				KEY_LEFTBRACKET = 47,
				KEY_RIGHTBRACKET = 48,
				KEY_BACKSLASH = 49,
				KEY_NONUSHASH = 50,
				KEY_SEMICOLON = 51,
				KEY_APOSTROPHE = 52,
				KEY_GRAVE = 53,
				KEY_COMMA = 54,
				KEY_PERIOD = 55,
				KEY_SLASH = 56,
				KEY_CAPSLOCK = 57,
				KEY_F1 = 58,
				KEY_F2 = 59,
				KEY_F3 = 60,
				KEY_F4 = 61,
				KEY_F5 = 62,
				KEY_F6 = 63,
				KEY_F7 = 64,
				KEY_F8 = 65,
				KEY_F9 = 66,
				KEY_F10 = 67,
				KEY_F11 = 68,
				KEY_F12 = 69,
				KEY_PRINTSCREEN = 70,
				KEY_SCROLLLOCK = 71,
				KEY_PAUSE = 72,
				KEY_INSERT = 73,
				KEY_HOME = 74,
				KEY_PAGEUP = 75,
				KEY_DELETE = 76,
				KEY_END = 77,
				KEY_PAGEDOWN = 78,
				KEY_RIGHT = 79,
				KEY_LEFT = 80,
				KEY_DOWN = 81,
				KEY_UP = 82,
				KEY_NUMLOCKCLEAR = 83,
				KEY_KP_DIVIDE = 84,
				KEY_KP_MULTIPLY = 85,
				KEY_KP_MINUS = 86,
				KEY_KP_PLUS = 87,
				KEY_KP_ENTER = 88,
				KEY_KP_1 = 89,
				KEY_KP_2 = 90,
				KEY_KP_3 = 91,
				KEY_KP_4 = 92,
				KEY_KP_5 = 93,
				KEY_KP_6 = 94,
				KEY_KP_7 = 95,
				KEY_KP_8 = 96,
				KEY_KP_9 = 97,
				KEY_KP_0 = 98,
				KEY_KP_PERIOD = 99,
				KEY_NONUSBACKSLASH = 100,
				KEY_APPLICATION = 101,
				KEY_POWER = 102,
				KEY_KP_EQUALS = 103,
				KEY_F13 = 104,
				KEY_F14 = 105,
				KEY_F15 = 106,
				KEY_F16 = 107,
				KEY_F17 = 108,
				KEY_F18 = 109,
				KEY_F19 = 110,
				KEY_F20 = 111,
				KEY_F21 = 112,
				KEY_F22 = 113,
				KEY_F23 = 114,
				KEY_F24 = 115,
				// KEY_EXECUTE = 116, <=== a defined macro, have to find another name
				KEY_HELP = 117,
				KEY_MENU = 118,
				KEY_SELECT = 119,
				KEY_STOP = 120,    
				KEY_AGAIN = 121,
				KEY_UNDO = 122, 
				KEY_CUT = 123,
				KEY_COPY = 124,
				KEY_PASTE = 125,
				KEY_FIND = 126,
				KEY_MUTE = 127,
				KEY_VOLUMEUP = 128,
				KEY_VOLUMEDOWN = 129,
				KEY_KP_COMMA = 133,
				KEY_KP_EQUALSAS400 = 134,
				KEY_INTERNATIONAL1 = 135,
				KEY_INTERNATIONAL2 = 136,
				KEY_INTERNATIONAL3 = 137,
				KEY_INTERNATIONAL4 = 138,
				KEY_INTERNATIONAL5 = 139,
				KEY_INTERNATIONAL6 = 140,
				KEY_INTERNATIONAL7 = 141,
				KEY_INTERNATIONAL8 = 142,
				KEY_INTERNATIONAL9 = 143,
				KEY_LANG1 = 144,
				KEY_LANG2 = 145,
				KEY_LANG3 = 146,
				KEY_LANG4 = 147,
				KEY_LANG5 = 148,
				KEY_LANG6 = 149,
				KEY_LANG7 = 150,
				KEY_LANG8 = 151,
				KEY_LANG9 = 152,
				KEY_ALTERASE = 153,
				KEY_SYSREQ = 154,
				KEY_CANCEL = 155,
				KEY_CLEAR = 156,
				KEY_PRIOR = 157,
				KEY_RETURN2 = 158,
				KEY_SEPARATOR = 159,
				KEY_OUT = 160,
				KEY_OPER = 161,
				KEY_CLEARAGAIN = 162,
				KEY_CRSEL = 163,
				KEY_EXSEL = 164,
				KEY_KP_00 = 176,
				KEY_KP_000 = 177,
				KEY_THOUSANDSSEPARATOR = 178,
				KEY_DECIMALSEPARATOR = 179,
				KEY_CURRENCYUNIT = 180,
				KEY_CURRENCYSUBUNIT = 181,
				KEY_KP_LEFTPAREN = 182,
				KEY_KP_RIGHTPAREN = 183,
				KEY_KP_LEFTBRACE = 184,
				KEY_KP_RIGHTBRACE = 185,
				KEY_KP_TAB = 186,
				KEY_KP_BACKSPACE = 187,
				KEY_KP_A = 188,
				KEY_KP_B = 189,
				KEY_KP_C = 190,
				KEY_KP_D = 191,
				KEY_KP_E = 192,
				KEY_KP_F = 193,
				KEY_KP_XOR = 194,
				KEY_KP_POWER = 195,
				KEY_KP_PERCENT = 196,
				KEY_KP_LESS = 197,
				KEY_KP_GREATER = 198,
				KEY_KP_AMPERSAND = 199,
				KEY_KP_DBLAMPERSAND = 200,
				KEY_KP_VERTICALBAR = 201,
				KEY_KP_DBLVERTICALBAR = 202,
				KEY_KP_COLON = 203,
				KEY_KP_HASH = 204,
				KEY_KP_SPACE = 205,
				KEY_KP_AT = 206,
				KEY_KP_EXCLAM = 207,
				KEY_KP_MEMSTORE = 208,
				KEY_KP_MEMRECALL = 209,
				KEY_KP_MEMCLEAR = 210,
				KEY_KP_MEMADD = 211,
				KEY_KP_MEMSUBTRACT = 212,
				KEY_KP_MEMMULTIPLY = 213,
				KEY_KP_MEMDIVIDE = 214,
				KEY_KP_PLUSMINUS = 215,
				KEY_KP_CLEAR = 216,
				KEY_KP_CLEARENTRY = 217,
				KEY_KP_BINARY = 218,
				KEY_KP_OCTAL = 219,
				KEY_KP_DECIMAL = 220,
				KEY_KP_HEXADECIMAL = 221,
				KEY_LCTRL = 224,
				KEY_LSHIFT = 225,
				KEY_LALT = 226,
				KEY_LGUI = 227,
				KEY_RCTRL = 228,
				KEY_RSHIFT = 229,
				KEY_RALT = 230,
				KEY_RGUI = 231,
				KEY_MODE = 257,
				KEY_AUDIONEXT = 258,
				KEY_AUDIOPREV = 259,
				KEY_AUDIOSTOP = 260,
				KEY_AUDIOPLAY = 261,
				KEY_AUDIOMUTE = 262,
				KEY_MEDIASELECT = 263,
				KEY_WWW = 264,
				KEY_MAIL = 265,
				KEY_CALCULATOR = 266,
				KEY_COMPUTER = 267,
				KEY_AC_SEARCH = 268,
				KEY_AC_HOME = 269,
				KEY_AC_BACK = 270,
				KEY_AC_FORWARD = 271,
				KEY_AC_STOP = 272,
				KEY_AC_REFRESH = 273,
				KEY_AC_BOOKMARKS = 274,
				KEY_BRIGHTNESSDOWN = 275,
				KEY_BRIGHTNESSUP = 276,
				KEY_DISPLAYSWITCH = 277,
				KEY_KBDILLUMTOGGLE = 278,
				KEY_KBDILLUMDOWN = 279,
				KEY_KBDILLUMUP = 280,
				KEY_EJECT = 281,
				KEY_SLEEP = 282,
				KEY_APP1 = 283,
				KEY_APP2 = 284,
				KEY_AUDIOREWIND = 285,
				KEY_AUDIOFASTFORWARD = 286,
				KEY_SOFTLEFT = 287,
				KEY_SOFTRIGHT = 288,
				KEY_CALL = 289,
				KEY_ENDCALL = 290,
				KEY_COUNT,
			};

			enum class KeyState{
				PRESSED,
				RELEASED
			};

			enum class KeymodBits{
				NONE = 		0,
				LSHIFT = 	1 << 0,
				RSHIFT = 	1 << 1,
				LCTRL = 	1 << 2,
				RCTRL = 	1 << 3,
				LALT = 		1 << 4,
				RALT = 		1 << 5,
				LGUI = 		1 << 6,
				RGUI = 		1 << 7,
				NUM = 		1 << 8,
				CAPS = 		1 << 9,
				MODE = 		1 << 10,
				SCROLL = 	1 << 11,

				CTRL = LCTRL | RCTRL,
				SHIFT = LSHIFT | RSHIFT,
				ALT = LALT | RALT,
				GUI = LGUI | RGUI,
			};
			RAINDROP_FLAGS_CLASS(Keymod, KeymodBits);

			enum class MouseButton{
				LEFT,
				MIDDLE,
				RIGHT,
				COUNT,
			};

			enum class ButtonState{
				DOWN,
				UP,
			};

			class Manager;
			class Listener;

			friend class Manager;
			friend class Listener;

			/**
			 * @brief Generic event identifier.
			 * 
			 */
			using ID = std::size_t;

			/**
			 * @brief Invalid identifier placeholder. 
			 * 
			 */
			static constexpr ID INVALID_ID = (ID)(~0);

			/**
			 * @brief When the event pool is empty, increase the original size by a factor of GROW_FACTOR plus GROW_CONSTANT
			 * 
			 * @example newSize = oldSize * GROW_FACTOR + GROW_CONSTANT
			 * 
			 */
			static constexpr std::size_t GROW_FACTOR = 2;
			static constexpr std::size_t GROW_CONSTANT = 10;
		
		protected:
			struct Subscriber{
				Listener* listener;
				void* callback;

				constexpr inline bool operator==(const Subscriber& other) const noexcept{
					return listener == other.listener;
				}

				struct hash{
					constexpr inline std::size_t operator()(const Subscriber& s) const noexcept{
						return reinterpret_cast<std::size_t>(s.listener);
					}
				};
			};
		
		public:
			
			/**
			 * @brief Listener class, subscribes to events and receives callbacks on triggers.
			 * 
			 */
			class Listener{
				public:
					/**
					 * @brief Construct a new Listener object
					 * 
					 * @param manager The event manager where listened event will be registred
					 */
					inline Listener() noexcept : _manager{nullptr}{}
					inline Listener(Manager& manager) noexcept : _manager{&manager}{}
					inline Listener(Context& context) noexcept : _manager(&_GetManager(context)){}

					static Listener Create(Context& context);

					/**
					 * @brief Destroy the Listener object and unsubscribes from all subscribed events
					 * 
					 */
					virtual ~Listener();

					// // Static events
					// virtual void OnFrame(){}
					// virtual void OnStartup(){}
					// virtual void OnShutdown(){}

					/**
					 * @brief Subscribes to an event
					 * 
					 * @tparam F the function callback type
					 * @param eventName the event's name to listen to
					 * @param callback the function called on the event trigger
					 */
					template<typename F>
					void subscribe(const std::string& eventName, const F& callback);

					/**
					 * @brief Subscribes to an event
					 * 
					 * @tparam F the function callback type
					 * @param eventID the event's ID to listen to
					 * @param callback the function called on the event trigger
					 */
					template<typename F>
					void subscribe(const Event::ID& eventID, const F& callback);

					/**
					 * @brief Subscribes to an event
					 * 
					 * @tparam F the function callback type
					 * @param event the event to listen to
					 * @param callback the function called on the event trigger
					 */
					template<typename F>
					void subscribe(Event& event, const F& callback);

					/**
					 * @brief Unsubscribes from an event
					 * 
					 * @param id the event's ID to unsubscribe from
					 */
					inline void unsubscribe(const Event::ID& id){
						unsubscribe(_manager->get(id));
					}

					/**
					 * @brief Unsubscribes from an event
					 * 
					 * @param name the event's name to unsubscribe from
					 */
					inline void unsubscribe(const std::string& name){
						unsubscribe(_manager->get(name));
					}

					/**
					 * @brief Unsubscribes from an event
					 * 
					 * @param event the event to unsubscribe from
					 */
					void unsubscribe(Event&& event);

				protected:
					Manager* _manager;
					
					void initialize(Context& context);
				
				private:
					std::unordered_set<ID> _subscribtions;
			};

			/**
			 * @brief The event registry. Stores events and link them to their name and id.
			 * 
			 */
			class Manager{
				friend class Event;
				public:
					/**
					 * @brief Construct a new Manager object
					 * 
					 */
					Manager(Internal::Context& context) : _context{context}{}

					/**
					 * @brief Destroy the Manager object
					 * 
					 */
					~Manager() = default;
					
					/**
					 * @brief Get an event from it's ID
					 * 
					 * @param id The ID of the event
					 * @return Event& 
					 */
					inline Event get(const Event::ID& id){
						return Event(this, id);
					}

					/**
					 * @brief Get an event from it's name
					 * 
					 * @param name The name of the event
					 * @return Event& 
					 */
					inline Event get(const std::string& name){
						return Event(this, getID(name));
					}

					/**
					 * @brief Get the ID of an event
					 * 
					 * @param name the name of the event
					 * @return const Event::ID& 
					 */
					inline const Event::ID& getID(const std::string& name){
						return _nameToIDMap.at(name);
					}

					/**
					 * @brief Creates a new event
					 * 
					 * @tparam Args the event callbacks's arguments
					 * @param name the name of the event
					 * @return Event
					 * 
					 * @throws std::runtime_error if the name is already used
					 * @throws std::bad_alloc if the pool reallocation failes
					 */
					template<typename... Args>
					Event create(const std::string& name);

					/**
					 * @brief Creates a new event
					 * 
					 * @param name the name of the event
					 * @return Event 
					 * 
					 * @throws std::runtime_error if the name is already used
					 * @throws std::bad_alloc if the pool reallocation failes
					 */
					Event create(const std::string& name);

					/**
					 * @brief Destroys an event
					 * 
					 * @param name the name of the event
					 */
					inline void destroy(const std::string& name){
						destroy(get(name));
					}

					/**
					 * @brief Destroys an event
					 * 
					 * @param id the ID of the event
					 */
					inline void destroy(const Event::ID& id){
						destroy(get(id));
					}

					/**
					 * @brief Destroys an event
					 * 
					 * @param event The event to destroy
					 */
					void destroy(const Event& event);

				private:
					struct EventData{
						ID id;
						std::string name;
						std::unordered_set<Subscriber, Subscriber::hash> subscribers;
						std::vector<std::type_index> arguments;

						template<typename... Args>
						void setArguments();

						void unsubscribe(Listener* listener);
						void subscribe(Listener* listener, void* fnc);
					};

					constexpr inline EventData& getData(const ID& id) noexcept{
						return _events[id];
					}

					Internal::Context& _context;
					std::unordered_map<std::string, Event::ID> _nameToIDMap;
					std::vector<EventData> _events;
					std::queue<Event::ID> _freeIds;
			};

			/**
			 * @brief Construct a new invalid Event object
			 * 
			 */
			constexpr Event() noexcept : _id{INVALID_ID}, _manager{nullptr}{}

			
			constexpr Event(Manager* manager, const ID& id) noexcept : _manager{manager}, _id{id}{}
			
			/**
			 * @brief Destroy the Event object
			 * 
			 */
			virtual ~Event() = default;
			
			/**
			 * @brief Calls all the subscribed callbacks with the given agruments
			 * 
			 * @tparam Args The arguments types
			 * @param args The arguments given to the callbacks
			 * 
			 * @throws std::invalid_argument If the argument count does not match the initial agurment counts
			 * @throws std::invalid_argument If the argument types do not match the initial arguments types
			 */
			template<typename... Args>
			void trigger(Args&&... args);

			/**
			 * @brief Subscribe to the event
			 * 
			 * @tparam F the callback function type
			 * @param listener The listener
			 * @param function the listener's callback
			 */
			template<typename F>
			void subscribe(Listener* listener, const F& function);

			/**
			 * @brief Unsubscribes a listener from the callback
			 * 
			 * @param listener the listener who's unsubscribing
			 */
			void unsubscribe(Listener* listener);

			/**
			 * @brief Get the event ID
			 * 
			 * @return constexpr const ID& 
			 */
			constexpr inline const ID& id() const noexcept{
				return _id;
			}

			/**
			 * @brief Get the event name
			 * 
			 * @return constexpr const std::string& 
			 */
			constexpr inline const std::string& name() const noexcept{
				return getData().name;
			}

			/**
			 * @brief Get the count of listeners subcribed to this event
			 * 
			 * @return constexpr const std::size_t& 
			 */
			inline std::size_t subscriberCount() const noexcept{
				return getData().subscribers.size();
			}


			template<typename... Args>
			static Event Create(Context& context, const std::string& name);
			static Event Create(Context& context, const std::string& name);

			static void Destroy(Context& context, const Event& event);
			static void Destroy(Context& context, const std::string& name);
			static void Destroy(Context& context, const ID& id);


			template<typename F>
			static void Subscribe(Context& context, Event& event, Listener* listener, const F& function);

			template<typename F>
			static void Subscribe(Context& context, const std::string& name, Listener* listener, const F& function);

			template<typename F>
			static void Subscribe(Context& context, const ID& id, Listener* listener, const F& function);

			static void Unsubscribe(Context& context, Event& event, Listener* listener);
			static void Unsubscribe(Context& context, const std::string& name, Listener* listener);
			static void Unsubscribe(Context& context, const ID& id, Listener* listener);

			template<typename... Args>
			static void Trigger(Context& context, Event& event, Args&&... args);
			
			template<typename... Args>
			static void Trigger(Context& context, const std::string& name, Args&&... args);

			template<typename... Args>
			static void Trigger(Context& context, const ID& id, Args&&... args);


		protected:

			static Manager& _GetManager(Context& context) noexcept;

			constexpr inline Manager::EventData& getData() noexcept{
				return _manager->getData(_id);
			}

			constexpr inline const Manager::EventData& getData() const noexcept{
				return _manager->getData(_id);
			}

			static std::string demangle(const std::type_index& i);

			template<typename... Args>
			void checkArgs() const;

			Manager* _manager;
			ID _id;
	};

	static Event CreateEvent(Context& context, const std::string& name){
		return Event::Create(context, name);
	}

	template<typename... Args>
	static Event CreateEvent(Context& context, const std::string& name){
		return Event::Create<Args...>(context, name);
	}

	static Event::Listener CreateEventListener(Context& context){
		return Event::Listener::Create(context);
	}
}

#include "Inline/Event.inl"

#endif