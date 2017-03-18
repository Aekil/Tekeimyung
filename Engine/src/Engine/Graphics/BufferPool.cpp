#include <Engine/Graphics/BufferPool.hpp>
#include <Engine/Utils/Debug.hpp>

void BufferPool::SubBuffer::free()
{
    chunk->freeSubBuffers.push_back(idx);
}

BufferPool::BufferPool(uint32_t countPerChunk, uint32_t subBufferSize, GLuint bufferType):
                        _countPerChunk(countPerChunk), _subBufferSize(subBufferSize), _bufferType(bufferType)
{
    GLint uboAlignment = 0;
    glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &uboAlignment);

    ASSERT(uboAlignment != 0, "The alignment should not be 0 (Is opengl initialized ?)");

    // Get aligned size of subBufferSize
    if (_subBufferSize % uboAlignment != 0) {
        _subBufferSize = _subBufferSize + uboAlignment - (_subBufferSize % uboAlignment);
    } else {
        _subBufferSize = _subBufferSize;
    }
}

BufferPool::~BufferPool()
{
    for (const auto& chunk: _chunks) {
        delete[] chunk->subBuffers;
    }
}

BufferPool::SubBuffer* BufferPool::allocate()
{
    // Search free sub-buffer
    {
        for (const auto& chunk: _chunks) {
            if (chunk->freeSubBuffers.size() != 0)
            {
                SubBuffer* subBuffer = chunk->subBuffers + chunk->freeSubBuffers.back();
                chunk->freeSubBuffers.pop_back();
                return (subBuffer);
            }
        }
    }

    // No free sub-buffer found, allocate new chunk
    std::unique_ptr<BufferPool::Chunk> chunk = std::make_unique<BufferPool::Chunk>();

    // Init new subBuffers chunk
    {
        chunk->ubo.init(_countPerChunk * _subBufferSize, _bufferType);
        chunk->ubo.setBindingPoint(3);
        chunk->subBuffers = new SubBuffer[_countPerChunk];
        chunk->freeSubBuffers.resize(_countPerChunk);
        for (uint32_t i = 0; i < _countPerChunk; ++i) {
            chunk->freeSubBuffers[i] = i;
            chunk->subBuffers[i].ubo = &chunk->ubo;
            chunk->subBuffers[i].size = _subBufferSize;
            chunk->subBuffers[i].offset = _subBufferSize * i;
            chunk->subBuffers[i].chunk = chunk.get();
            chunk->subBuffers[i].idx = i;
        }
    }

    _chunks.push_back(std::move(chunk));

    return (allocate());
}

void BufferPool::free(SubBuffer* subBuffer)
{
    subBuffer->free();
}
