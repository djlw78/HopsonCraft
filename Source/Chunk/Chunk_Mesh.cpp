#include "Chunk_Mesh.h"

#include "Chunk.h"
#include "Texture.h"

#include <iostream>

void Chunk_Mesh::Mesh_Part::addVerticies(std::vector<GLfloat> verticies )
{
    vertexCoords.insert(vertexCoords.end(), verticies.begin(), verticies.end());
}

void Chunk_Mesh::Mesh_Part::addUvCoords (std::vector<GLfloat> coords)
{
    textureCoords.insert(textureCoords.end(), coords.begin(), coords.end());
}

Chunk_Mesh::Chunk_Mesh(const Chunk& chunk, const std::vector<Chunk_Layer>& layers)
:   m_p_chunk (&chunk)
,   m_p_layers(&layers)
{}

void Chunk_Mesh::bufferMesh()
{
    std::cout << "Verts: " << m_chunkModel.vertexCoords.size() << std::endl;
    std::cout << "UVs:   " << m_chunkModel.textureCoords.size() << std::endl;
    m_chunkModel.model.create(m_chunkModel.vertexCoords, m_chunkModel.textureCoords);
    m_chunkModel.textureCoords.clear();
    m_chunkModel.vertexCoords.clear();
}

void Chunk_Mesh::bind()
{
    m_chunkModel.model.bind();
}

void Chunk_Mesh::draw()
{
    glDrawArrays(GL_TRIANGLES, 0, m_chunkModel.model.getVertexCount());
}



void Chunk_Mesh::generateMesh()
{
    std::cout << "Generating mesh" << std::endl;
    for (unsigned y = 0 ; y < m_p_layers->size() - 1 ; y++) {
        for (unsigned z = 0 ; z < Chunk::SIZE ; z++) {
            for (unsigned x = 0 ; x < Chunk::SIZE ; x++) {
                if (m_p_layers->at(y).getBlock({x, z}).getID() == Block::Id::Air) {
                    continue;
                } else {
                    addBlockMesh (x, y, z, m_p_layers->at(y).getBlock({x, z}));
                }
            }
        }
    }
}


void Chunk_Mesh::addBlockMesh (float x, float y, float z, const Block::Block_Base& block)
{
    auto isAir = [&](int x, int y, int z)
    {
        return m_p_chunk->getBlock (x, y, z).getID() == Block::Id::Air;
    };

    if (isAir(x, y + 1, z)) {
        addBlockTopToMesh   (x, y, z, block);
    }
    if (isAir(x, y -1, z)) {
        addBlockBottomToMesh(x, y, z, block);
    }
    if (isAir(x - 1, y, z)) {
        addBlockLeftToMesh  (x, y, z, block);
    }
    if (isAir(x + 1, y, z)) {
        addBlockRightToMesh (x, y, z, block);
    }
    if (isAir(x, y, z + 1)) {
        addBlockFrontToMesh (x, y, z, block);
    }
    if (isAir(x, y, z - 1)) {
        addBlockBackToMesh  (x, y, z, block);
    }
}


void Chunk_Mesh::addBlockTopToMesh(float x, float y, float z, const Block::Block_Base& block)
{
    m_chunkModel.addVerticies
    ({
        x,      y + 1, z + 1,
        x + 1,  y + 1, z + 1,
        x + 1,  y + 1, z,
        x + 1,  y + 1, z,
        x,      y + 1, z,
        x,      y + 1, z + 1,
    });
    m_chunkModel.addUvCoords(m_p_chunk->getAtlas().getTextureCoords(block.getTextureTop()));
}

void Chunk_Mesh::addBlockBottomToMesh(float x, float y, float z, const Block::Block_Base& block)
{
    m_chunkModel.addVerticies
    ({
        x,      y, z,
        x + 1,  y, z,
        x + 1,  y, z + 1,
        x + 1,  y, z + 1,
        x,      y, z + 1,
        x,      y, z
    });
    m_chunkModel.addUvCoords(m_p_chunk->getAtlas().getTextureCoords(block.getTextureBottom()));
}

void Chunk_Mesh::addBlockLeftToMesh(float x, float y, float z, const Block::Block_Base& block)
{
    m_chunkModel.addVerticies
    ({
        x, y,       z,
        x, y,       z + 1,
        x, y + 1,   z + 1,
        x, y + 1,   z + 1,
        x, y + 1,   z,
        x, y,       z,
    });
    m_chunkModel.addUvCoords(m_p_chunk->getAtlas().getTextureCoords(block.getTextureSide()));
}

void Chunk_Mesh::addBlockRightToMesh(float x, float y, float z, const Block::Block_Base& block)
{
    m_chunkModel.addVerticies
    ({
        x + 1, y,       z + 1,
        x + 1, y,       z,
        x + 1, y + 1,   z,
        x + 1, y + 1,   z,
        x + 1, y + 1,   z + 1,
        x + 1, y,       z + 1,
    });
    m_chunkModel.addUvCoords(m_p_chunk->getAtlas().getTextureCoords(block.getTextureSide()));
}

void Chunk_Mesh::addBlockFrontToMesh(float x, float y, float z, const Block::Block_Base& block)
{
    m_chunkModel.addVerticies
    ({
        x,      y,      z + 1,
        x + 1,  y,      z + 1,
        x + 1,  y + 1,  z + 1,
        x + 1,  y + 1,  z + 1,
        x,      y + 1,  z + 1,
        x,      y,      z + 1,
    });
    m_chunkModel.addUvCoords(m_p_chunk->getAtlas().getTextureCoords(block.getTextureSide()));
}

void Chunk_Mesh::addBlockBackToMesh(float x, float y, float z, const Block::Block_Base& block)
{
    m_chunkModel.addVerticies
    ({
        x + 1,  y,      z,
        x,      y,      z,
        x,      y + 1,  z,
        x,      y + 1,  z,
        x + 1,  y + 1,  z,
        x + 1,  y,      z,
    });
    m_chunkModel.addUvCoords(m_p_chunk->getAtlas().getTextureCoords(block.getTextureSide()));
}