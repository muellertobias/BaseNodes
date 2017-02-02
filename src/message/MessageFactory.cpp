/*
 * MessageFactory.cpp
 *
 *  Created on: 25.01.2017
 *      Author: tobias
 */

#include "MessageFactory.h"

#include <map>

#include "../helper/exception/NodeBaseException.h"
#include "../helper/time/VectorTime.h"
#include "ApplicationMessage.h"
#include "ControlMessage.h"
#include "Message.h"

namespace message {

Message* MessageFactory::create(const string& xmlRawData) {
	if (xmlRawData.empty()) {
		throw new helper::exception::NodeBaseException("Message creation failed! StringContent empty");
	}
	return read(xmlRawData);
}

string MessageFactory::convertToString(Message* const message) {
	return write(message);
}

Message* MessageFactory::read(const string& xmlRawData) {

	// TODO Error Handling
	// XML
	XMLDocument doc;
	doc.Parse(xmlRawData.c_str(), xmlRawData.length());

	Message* message = nullptr;
	int number = 0;
	int sourceID = 0;
	int destinationID = 0;
	MessageSubType subType;
	VectorTime vectorTime;
	string content;

	XMLElement* root = doc.FirstChild()->ToElement();
	if (root == nullptr)
		return nullptr;

	root->QueryAttribute("Number", &number);

	XMLElement* element;

	int _subType = 0;
	element = root->FirstChildElement("Type");
	element->QueryIntText(&_subType);
	subType = static_cast<MessageSubType>(_subType);

	element = root->FirstChildElement("SourceID");
	element->QueryIntText(&sourceID);

	element = root->FirstChildElement("DestinationID");
	element->QueryIntText(&destinationID);

	element = root->FirstChildElement("VectorTimes");
	for (XMLElement* timeElement = element->FirstChildElement("Time"); timeElement != NULL; timeElement = timeElement->NextSiblingElement("Time")) {
		int id = stoi(timeElement->Attribute("NodeID"));
		int time = stoi(timeElement->GetText());
		vectorTime.setTime(id, time);
	}

	element = root->FirstChildElement("Content");
	if(strlen(element->GetText()) > 0) {
		content = string(element->GetText());
	}

	if (strcmp(root->Name(), "ControlMessage") == 0) {
		message = new ControlMessage(subType, number, sourceID, content);
	} else if (strcmp(root->Name(), "ApplicationMessage") == 0) {
		message = new ApplicationMessage(subType, number, sourceID, content);
	}
	message->setVectorTimes(vectorTime.getTimeMap());
	message->setDestinationId(destinationID);
	return message;
}

string MessageFactory::write(Message* const message) {
	// TODO Error Handling
	string returnString;

	XMLDocument doc;
	XMLElement* root = writeXMLElement(message, doc);
	doc.InsertEndChild(root);

	XMLPrinter p;
	doc.Print(&p);

	returnString.append(p.CStr());

	return returnString;
}

XMLElement* MessageFactory::writeXMLElement(Message* const message, XMLDocument& doc) {

	string rootName;
	if (dynamic_cast<ControlMessage*>(message) != NULL) {
		rootName = "ControlMessage";
	} else if (dynamic_cast<ApplicationMessage*>(message) != NULL) {
		rootName = "ApplicationMessage";
	}

	XMLElement* root = doc.NewElement(rootName.c_str());
	root->SetAttribute("Number", message->getNumber());

	XMLElement* type = doc.NewElement("Type");
	type->SetText(message->getType());
	root->InsertEndChild(type);

	XMLElement* sourceID = doc.NewElement("SourceID");
	sourceID->SetText(message->getSourceID());
	root->InsertEndChild(sourceID);

	XMLElement* destinationID = doc.NewElement("DestinationID");
	destinationID->SetText(message->getDestinationId());
	root->InsertEndChild(destinationID);

	XMLElement* xvectorTimes = doc.NewElement("VectorTimes");
	for (VectorTimeMap::const_iterator it = message->getVectorTimes().begin(); it != message->getVectorTimes().end(); ++it) {
		XMLElement* element = doc.NewElement("Time");
		element->SetAttribute("NodeID", it->first);
		element->SetText(it->second);
		xvectorTimes->InsertEndChild(element);
	}
	root->InsertEndChild(xvectorTimes);

	XMLElement* content = doc.NewElement("Content");
	content->SetText(message->getContent().c_str());
	root->InsertEndChild(content);

	return root;
}

} /* namespace message */

