#pragma once
#include <Core/Base/PublicSingleton.h>
#include <qapplication.h>
namespace Soarscape
{
	class Engine;
	class Editor : public PublicSingleton<Editor>
	{
	public:
		void initialize(int argc, char* argv[]);
		void run();

	private:
		Engine* m_RunTimeEngine;
		std::unique_ptr<QApplication> m_QApp;
	};
} // namespace Soarscape
