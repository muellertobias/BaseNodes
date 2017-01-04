Nachrichtenformat

Message::Type::Number::Source::Payload
	    Control
	    	  Shutdown
	    
	    Application
	          Content
	          
	          
	          
Beispiele:

Herunterfahren aller Knoten per Weiterleitung
Message::Control::1234::-1::Shutdown

Herunterfahren eines Knoten (z.B. 3)
Message::Control::1234::3::Shutdown

Schnappschuss eines Knoten
Message::Control::1234::1::Snapshot

Anwendungsnachricht
Message::Application::2345::1::Hallo Knoten