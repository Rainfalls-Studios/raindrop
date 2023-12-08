#include <Raindrop/Graphics/Framebuffer/FramebufferManager.hpp>
#include <Raindrop/Graphics/Framebuffer/Framebuffer.hpp>
#include <Raindrop/Graphics/Framebuffer/FramebufferBuilder.hpp>

namespace Raindrop::Graphics::Framebuffer{
	FramebufferManager::FramebufferManager(Context& context) : _context{context}{}
	FramebufferManager::~FramebufferManager(){}

	std::shared_ptr<Framebuffer> FramebufferManager::create(FramebufferBuilder& builder){
		std::shared_ptr<Framebuffer> framebuffer = std::make_shared<Framebuffer>(_context, builder);
		_framebuffers[builder.name()] = framebuffer;
		return framebuffer;
	}

	std::shared_ptr<Framebuffer> FramebufferManager::get(std::string name){
		return _framebuffers[name];
	}

	void FramebufferManager::clean(){
		auto it = _framebuffers.begin();

		while (it != _framebuffers.end()){
			if (it->second.unique()){
				_framebuffers.erase(it);
				continue;
			}
			it++;
		}
	}
}