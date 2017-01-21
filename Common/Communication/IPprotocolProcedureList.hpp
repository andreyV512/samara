#pragma once 

#include "templates/typelist.hpp"
#include "ServiceProtocol.h"

typedef TL::MkTlst<
	StatusProtocol::Server		
	, RunProtocol::Server	
	, StopProtocol::Server	
>::Result __ip_protocol_procedure_list__;