/*
 * Message.cpp
 *
 *  Created on: 01.11.2016
 *      Author: tobias
 */

#include "Message.h"
#include "../helper/string/trim.h"
#include "../helper/utilities/tinyxml2.h"

#include <iostream>

namespace network {

using namespace tinyxml2;

Message::Message(const string& str) {
	if (!read(str)) {
		cerr << "Invalid argument!" << endl;
		//throw new invalid_argument("str"); // Austauschen gegen eigene Exception beim decoden
	}
}

Message::Message(const MessageType& type, int number ,int sourceID, const string& content)
	: type(type), number(number), sourceID(sourceID),  content(content) {
}

Message::~Message() {
}

bool Message::read(const string& str) {
	// TODO Error Handling
	// XML
	XMLDocument doc;
	doc.Parse(str.c_str(), str.length());

	XMLElement* root = doc.FirstChild()->ToElement();
	if (root == nullptr)
		return false;

	root->QueryAttribute("Number", &this->number);

	XMLElement* element;

	element = root->FirstChildElement("Type");
	int type;
	element->QueryIntText(&type);
	this->type = static_cast<MessageType>(type);

	element = root->FirstChildElement("SourceID");
	element->QueryIntText(&this->sourceID);

	element = root->FirstChildElement("Content");
	this->content = string(element->GetText());

	return true;
}

string Message::write() const {
	// TODO Error Handling
	string returnString;

	XMLDocument doc;
	XMLElement* root = doc.NewElement("Message");
	root->SetAttribute("Number", this->getNumber());

	XMLElement* type = doc.NewElement("Type");
	type->SetText(this->getType());
	root->InsertEndChild(type);

	XMLElement* sourceID = doc.NewElement("SourceID");
	sourceID->SetText(this->getSourceID());
	root->InsertEndChild(sourceID);

	XMLElement* content = doc.NewElement("Content");
	content->SetText(this->getContent().c_str());
	root->InsertEndChild(content);

	doc.InsertEndChild(root);

	XMLPrinter p;
	doc.Print(&p);

	returnString.append(p.CStr());

	return returnString;
}

} /* namespace network */
