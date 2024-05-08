/**
 * @brief This code sample is what the API should look like, what we are aming form in term of API
 * @note This code does not work and is only here to give a "direction"
 * 
 */

Raindrop::Context context = Raindrop::CreateContext();
context.setName("sample");
context.initialize();

Raindrop::GUID::CreateStorage<Raindrop::RenderPass>(context);

Raindrop::RenderPass renderPass = Raindrop::CreateRenderPass(context);
renderPass.addAttachment(0, Raindrop::Format::RGBA8, Raindrop::Texture::Operation::Default);
renderPass.addAttachment(1, Raindrop::Format::D32, Raindrop::Texture::Operation::Default);
Raindrop::GUID::Store<Raindrop::RenderPass>(context, renerPass);

Raindrop::Subpass subpass = renderPass.addSubpass();

#include "TextureLoader.hpp"
Raindrop::Asset::CreateLoader<TextureLoader>(context, "Texture");
Raindrop::Texture = Raindrop::Asset::Load<Texture>(context, "Texture", "images/image.jpg");

subpass.setColorAttachments(0);
subpass.setDepth(1);

subpass.waitFor(
	Raindrop::Subpass::External,
	Raindrop::Stage::COLOR_OUTPUT | Raindrop::Stage::EARLY_TESTS
	Raindrop::Access::NONE,
	Raindrop::Stage::FRAGMENT_SHADER,
	Raindrop::Access::SHADER_READ
);

renderPass.initialize();

Raindrop::Format colorFormat = Raindrop::Format::find(
	{
		Raindrop::Format::Criteria::FLOAT,
		Raindrop::Format::Criteria::BIT16,
		Raindrop::Format::Criteria::BIT32,
		Raindrop::Format::Criteria::BIT64,
		Raindrop::Format::Criteria::R,
		Raindrop::Format::Criteria::G,
		Raindrop::Format::Criteria::B,
		Raindrop::Format::Criteria::COLOR
	},
	{
		Raindrop::Format::Feature::SAMPLED,
		Raindrop::Format::Feature::COLOR_ATTACHMENT
	}
);

Raindrop::Texture color = Raindrop::CreateTexture(context, 1080, 720, colorFormat, Raindrop::Texture::Usage::COLOR_ATTACHMENT | Raindrop::Usage::SAMPLED);
color.setTiling(Raindrop::Texture::Tiling::OPTIMAL);
color.setlayout(Raindrop::Texture::Layout::COLOR_ATTACHMENT);

color.setType(Raindrop::Texture::Type::PLANAR);

color.initialize();

Raindrop::TextureSubset colorSubset = Raindrop::CreateTextureSubset(context, color);
colorSubset.setRange({
	.aspect = Raindrop::Texture::Aspect::COLOR,
	.mipLevel = 0,
	.mipCount = 1,
	.layer = 0,
	.layerCount = 1
});

colorSubset.swizzle(Raindrop::ComponentSwizzle::R, Raindrop::ComponentSwizzle::R);
colorSubset.setType(Raindrop::TextureSubset::Type::PLANAR);
/*
    Linear
    Planar
    Volumetric
    Cubemap
    LinearArray
    PlanarArray
    CubemapArray
*/


Raindrop::Texture depth = Raindrop::CreateTexture(context, 1080, 720, Raindrop::Format::D32, Raindrop::Usage::DEPTH_ATTACHMENT);
depth.setTiling(Raindrop::Texture::Tiling::OPTIMAL);
depth.setlayout(Raindrop::Texture::Layout::DEPTH_ATTACHMENT);

depth.initialize();

Raindrop::Framebuffer framebuffer = Raindrop::CreateFramebuffer(context, renderPass, 1080, 720, {color, depth});
framebuffer.initialize();

Raindrop::Event event = Raindrop::Event::Register<int, char, char>("My event");

Raindrop::Event::Subscribe(eventID, [](int, char, char){...});
event.subscribe("My event", [](int, char, char){...});

event.trigger()


