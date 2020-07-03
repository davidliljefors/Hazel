#pragma once

#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv)
{
	Hazel::Log::Init();
	auto app = Hazel::CreateApplication();
	HZ_PROFILE_BEGIN_SESSION("Startup", "HazelProfile-runtime.json");
	app->Run();
	HZ_PROFILE_END_SESSION();
	delete app;
}

#endif