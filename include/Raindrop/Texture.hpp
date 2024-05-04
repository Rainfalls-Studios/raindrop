#ifndef __RAINDROP_TEXTURE_HPP__
#define __RAINDROP_TEXTURE_HPP__

#include "common.hpp"
#include "Format.hpp"
#include "Utils/Flags.hpp"

namespace Raindrop{
	class Texture{
		public:
			class Aspect : public Utils::FlagsTemplate<Aspect> {
				public:
					using FlagsTemplate<Aspect>::FlagsTemplate;

					enum Bits : Bitset {
						NONE = 0,
						COLOR = 1 << 0,
						DEPTH = 1 << 1,
						STENCIL = 1 << 2,
						METADATA = 1 << 3,
						PLANE_0 = 1 << 4,
						PLANE_1 = 1 << 5,
						PLANE_2 = 1 << 6,
						MEMORY_PLANE_0 = 1 << 7,
						MEMORY_PLANE_1 = 1 << 8,
						MEMORY_PLANE_2 = 1 << 9,
						MEMORY_PLANE_3 = 1 << 10
					};
			};

			class Operation{
				public:
					enum Load{
						LOAD,
						CLEAR,
						DONT_LOAD
					};

					enum Store{
						STORE,
						DONT_STORE
					};

					Operation(Load loadOp, Store storeOp, Load stencilLoadOp, Store stentilStorOp) noexcept;

					const Load& getLoadOp() const noexcept;
					const Store& getStoreOp() const noexcept;
					const Load& getStencilLoadOp() const noexcept;
					const Store& getStencilStoreOp() const noexcept;

					void setLoadOp(const Load& op) noexcept;
					void setStoreOp(const Store& op) noexcept;
					void setStencilLoadOp(const Load& op) noexcept;
					void setStencilStoreOp(const Store& op) noexcept;

				private:
					Load _loadOp;
					Load _stencilLoadOp;
					Store _storeOp;
					Store _stencilStoreOp;

				public:
					static const Operation DEFAULT;
			};

			enum class Tiling{
				OPTIMAL,
				LINEAR
			};

			enum class Layout{
				UNDEFINED = 0,
				GENERAL = 1,
				COLOR_ATTACHMENT_OPTIMAL = 2,
				DEPTH_STENCIL_ATTACHMENT_OPTIMAL = 3,
				DEPTH_STENCIL_READ_ONLY_OPTIMAL = 4,
				SHADER_READ_ONLY_OPTIMAL = 5,
				TRANSFER_SRC_OPTIMAL = 6,
				TRANSFER_DST_OPTIMAL = 7,
				PREINITIALIZED = 8,
				DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL = 1000117000,
				DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL = 1000117001,
				DEPTH_ATTACHMENT_OPTIMAL = 1000241000,
				DEPTH_READ_ONLY_OPTIMAL = 1000241001,
				STENCIL_ATTACHMENT_OPTIMAL = 1000241002,
				STENCIL_READ_ONLY_OPTIMAL = 1000241003,
				READ_ONLY_OPTIMAL = 1000314000,
				ATTACHMENT_OPTIMAL = 1000314001,
				PRESENT_SRC = 1000001002,
				VIDEO_DECODE_DST = 1000024000,
				VIDEO_DECODE_SRC = 1000024001,
				VIDEO_DECODE_DPB = 1000024002,
				SHARED_PRESENT = 1000111000,
				FRAGMENT_DENSITY_MAP_OPTIMAL = 1000218000,
				FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL = 1000164003,
				RENDERING_LOCAL_READ = 1000232000,
				VIDEO_ENCODE_DST = 1000299000,
				VIDEO_ENCODE_SRC = 1000299001,
				VIDEO_ENCODE_DPB = 1000299002,
				ATTACHMENT_FEEDBACK_LOOP_OPTIMAL = 1000339000
			};

			enum class Type{
				LINEAR,
				PLANAR,
				VOLUMETRIC
			};

			class Usage : public Utils::FlagsTemplate<Usage> {
				public:
					using FlagsTemplate<Usage>::FlagsTemplate;

					enum Bits : Bitset {
						NONE = 0,
						TRANSFER_SRC = 1 << 0,
						TRANSFER_DST = 1 << 1,
						SAMPLED = 1 << 2,
						STORAGE = 1 << 3,
						COLOR_ATTACHMENT = 1 << 4,
						DEPTH_STENCIL_ATTACHMENT = 1 << 5,
						TRANSIENT_ATTACHMENT = 1 << 6,
						INPUT_ATTACHMENT = 1 << 7,
						VIDEO_DECODE_DST = 1 << 8,
						VIDEO_DECODE_SRC = 1 << 9,
						VIDEO_DECODE_DPB = 1 << 10,
						FRAGMENT_DENSITY_MAP = 1 << 11,
						FRAGMENT_SHADING_RATE_ATTACHMENT = 1 << 12,
						HOST_TRANSFER = 1 << 13,
						VIDEO_ENCODE_DST = 1 << 14,
						VIDEO_ENCODE_SRC = 1 << 15,
						VIDEO_ENCODE_DPB = 1 << 16,
						ATTACHMENT_FEEDBACK_LOOP = 1 << 17,
						INVOCATION_MASK = 1 << 18,
						SAMPLE_WEIGHT = 1 << 19,
						SAMPLE_BLOCK_MATCH = 1 << 20,
						SHADING_RATE_IMAGE = 1 << 21
					};
			};


			class Flags : public Utils::FlagsTemplate<Flags> {
				public:
					using FlagsTemplate<Flags>::FlagsTemplate;

					enum Bits : Bitset {
						NONE = 0,
						SPARSE_BINDING = 1 << 0,
						SPARSE_RESIDENCY = 1 << 1,
						SPARSE_ALIASED = 1 << 2,
						MUTABLE_FORMAT = 1 << 3,
						CUBE_COMPATIBLE = 1 << 4,
						ALIAS = 1 << 5,
						SPLIT_INSTANCE_BIND_REGIONS = 1 << 6,
						PLANAR_ARRAY_COMPATIBLE = 1 << 7,
						BLOCK_TEXEL_VIEW_COMPATIBLE = 1 << 8,
						EXTENDED_USAGE = 1 << 9,
						PROTECTED = 1 << 10,
						DISJOINT = 1 << 11,
						CORNER_SAMPLED = 1 << 12,
						SAMPLE_LOCATIONS_COMPATIBLE_DEPTH = 1 << 13,
						SUBSAMPLED = 1 << 14,
						DESCRIPTOR_BUFFER_CAPTURE_REPLAY = 1 << 15,
						MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED = 1 << 16,
						FRAGMENT_DENSITY_MAP_OFFSET = 1 << 17,
						VIDEO_PROFILE_INDEPENDENT = 1 << 18,
					};
			};

			static Texture Create(Context& context, std::size_t width, std::size_t height, const Format& format, const Usage& usage);
			
			Texture();
			Texture(const Texture& other);
			Texture& operator=(const Texture& other);

			void initialize();
			void release();

			void setFormat(const Format& format);
			void setWidth(const std::size_t& width);
			void setHeight(const std::size_t& height);
			void setDepth(const std::size_t& depth);
			void setUsage(const Usage& usage);
			void setLayout(const Layout& layout);
			void setTiling(const Tiling& tiling);
			void setType(const Type& type);
			void setMipmapCount(const std::size_t mip);
			void setArrayLayers(const std::size_t layers);
			void setFlags(const Flags& flags);

			const Format& getFormat() const noexcept;
			const std::size_t& getWidth() const noexcept;
			const std::size_t& getHeight() const noexcept;
			const std::size_t& getDepth() const noexcept;
			const Usage& getUsage() const noexcept;
			const Layout& getLayout() const noexcept;
			const Tiling& getTiling() const noexcept;
			const Type& getType() const noexcept;
			const std::size_t& getMipmapCount() const noexcept;
			const std::size_t& getArrayLayers() const noexcept;
			const Flags& getFlags() const noexcept;

			bool isInitialized() const noexcept;
			void* getNativeHandle() const noexcept;
			// TextureID getHandle() const noexcept; TODO: unique runtime texture scope id
			// InstanceID getInstanceID() const noexcept; TODO: unique runtime engine scope id

		private:

			// TextureID _handle;
	};

	inline static Texture CreateTexture(Context& context, std::size_t width, std::size_t height, const Format& format, const Texture::Usage& usage){
		return Texture::Create(context, width, height, format, usage);
	}
}

#endif