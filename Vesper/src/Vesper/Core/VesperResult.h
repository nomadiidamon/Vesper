#pragma once

#include "Enums.h"
#include <string>

namespace Vesper {

	struct VesperResult
	{
		VZ_RETURN Return = VZ_RETURN::SUCCESS;
		std::string ResultMessage = "";

		VesperResult() = default;
		VesperResult(VZ_RETURN RETURN, std::string MESSAGE)
			: Return(RETURN), ResultMessage(MESSAGE) {
		}

		bool IsSuccess()
		{
			return Return == VZ_RETURN::SUCCESS;
		}

		std::string ReturnError()
		{
			std::string Error = "";
			switch (Return)
			{
			case VZ_RETURN::FAILURE:
				Error = "FAILURE";
				break;
			case VZ_RETURN::FILE_NOT_FOUND:
				Error = "FILE_NOT_FOUND";
				break;
			case VZ_RETURN::INVALID_ARGUMENT:
				Error = "INVALID_ARGUMENT";
				break;
			case VZ_RETURN::OUT_OF_MEMORY:
				Error = "OUT_OF_MEMORY";
				break;
			case VZ_RETURN::UNKNOWN_ERROR:
				Error = "UNKNOWN_ERROR";
				break;
			default:
				Error = "Undefined VZ_RETURN";
			}
			return Error;
		}
	};
}
