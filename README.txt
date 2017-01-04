Nachrichtenformat

Message::Type::Number::Source::Payload
	    Control
	    	  Shutdown
	    
	    Application
	          Content
	          
<Message Number="">
	<Type></Type>
	<SourceID></SourceID>
	<Content></Content>
</Message>
	          
	          
Beispiele:

Herunterfahren aller Knoten per Weiterleitung
<Message Number="1234">
	<Type>1</Type>
	<SourceID>-1</SourceID>
	<Content>Shutdown</Content>
</Message>

Herunterfahren eines Knoten (z.B. 3)
<Message Number="1234">
	<Type>1</Type>
	<SourceID>3</SourceID>
	<Content>Shutdown</Content>
</Message>

Schnappschuss eines Knoten
<Message Number="1234">
	<Type>1</Type>
	<SourceID>1</SourceID>
	<Content>Snapshot</Content>
</Message>

Anwendungsnachricht mit der Nachricht "Hello Node"
<Message Number="1234">
	<Type>2</Type>
	<SourceID>1</SourceID>
	<Content>Hello Node</Content>
</Message>


