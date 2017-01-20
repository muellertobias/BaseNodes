/*
 * Message.cpp
 *
 *  Created on: 01.11.2016
 *      Author: tobias
 */

#include "Message.h"
#include "../helper/string/trim.h"
#include "../helper/utilities/tinyxml2.h"


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

	element = root->FirstChildElement("VectorTimes");
	for (XMLElement* timeElement = element->FirstChildElement("Time"); timeElement != NULL; timeElement = timeElement->NextSiblingElement("Time")) {
		int id = stoi(timeElement->Attribute("NodeID"));
		int time = stoi(timeElement->GetText());
		setVectorTime(id, time);
	}

	element = root->FirstChildElement("Content");
	this->content = string(element->GetText());

	return true;
}

string Message::write() const {
	// TODO Error Handling
	string returnString;

	XMLDocument doc;
	XMLElement* root = writeXMLElement(doc);
	doc.InsertEndChild(root);

	XMLPrinter p;
	doc.Print(&p);

	returnString.append(p.CStr());

	return returnString;
}

XMLElement* Message::writeXMLElement(XMLDocument& doc) const {
	XMLElement* root = doc.NewElement("Message");
	root->SetAttribute("Number", this->getNumber());

	XMLElement* type = doc.NewElement("Type");
	type->SetText(this->getType());
	root->InsertEndChild(type);

	XMLElement* sourceID = doc.NewElement("SourceID");
	sourceID->SetText(this->getSourceID());
	root->InsertEndChild(sourceID);

	XMLElement* xvectorTimes = doc.NewElement("VectorTimes");
	for (VectorTimeMap::const_iterator it = vectorTime.getTimeMap().begin(); it != vectorTime.getTimeMap().end(); ++it) {
		XMLElement* element = doc.NewElement("Time");
		element->SetAttribute("NodeID", it->first);
		element->SetText(it->second);
		xvectorTimes->InsertEndChild(element);
	}
	root->InsertEndChild(xvectorTimes);

	XMLElement* content = doc.NewElement("Content");
	content->SetText(this->getContent().c_str());
	root->InsertEndChild(content);

	return root;
}

string Message::toString() const {
	//TODO StringStream statt string+=
	string str;
	str.clear();
	if (this->type == MessageType::application) {
		str = "Application -";
	} else if (this->type == MessageType::control) {
		str = "Control -";
	} else if (this->type == MessageType::log) {
			str = "Logging -";
	} else {
		str = "Undefined -";
	}
	str += " Number=" + to_string(this->number) + " SourceID=" + to_string(this->sourceID) + " Content={" + this->content + "} VectorTimes={";

	for (VectorTimeMap::const_iterator it = vectorTime.getTimeMap().begin(); it != vectorTime.getTimeMap().end(); it++) {
		str += "[" + to_string((*it).first) + "," + to_string((*it).second) + "]";
	}
	str += "}";

	return str;
	}

bool Message::setVectorTime(const int& nodeID, const int& time) {
	cout << nodeID << "/" << time << endl;
	vectorTime.setTime(nodeID, time);

	return true;

}

} /* namespace network */


