#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <Engine/Graphics/UniformBuffer.hpp>

class BufferPool {
struct Chunk;

public:
    class SubBuffer{
        friend BufferPool;

    public:
        SubBuffer() = default;
        ~SubBuffer() = default;

        SubBuffer(const SubBuffer&) = default;
        SubBuffer(SubBuffer&& subBuffer) = default;

        SubBuffer& operator=(const SubBuffer&) = default;
        SubBuffer& operator=(SubBuffer&& subBuffer) = default;

        void free();

    public:
        UniformBuffer* ubo;
        uint32_t size;
        uint32_t offset;

    private:
        Chunk* chunk;

        // Index in SubBuffer* array
        uint32_t idx;
    };

private:
    struct Chunk {
        UniformBuffer ubo;

        SubBuffer* subBuffers;

        // Store indices of free subBuffers
        std::vector<uint32_t> freeSubBuffers;
    };

public:
    BufferPool(uint32_t countPerChunk, uint32_t subBufferSize, GLuint bufferType = GL_UNIFORM_BUFFER);
    ~BufferPool();

    SubBuffer*                          allocate();
    void                                free(SubBuffer* buffer);

private:
    uint32_t                            _countPerChunk;
    uint32_t                            _subBufferSize;
    std::vector<std::unique_ptr<Chunk> > _chunks;
    GLuint                              _bufferType;
};
