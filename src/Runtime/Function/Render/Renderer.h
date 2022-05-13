#pragma once
#include "Core/Base/PublicSingleton.h"
namespace Soarscape
{
	class Renderer : public PublicSingleton<Renderer>
	{
	public:

		Renderer();

		void initialize();
		void begin();
		void end(uint32_t defaultFramebuffer);
	private:


	};

	
}