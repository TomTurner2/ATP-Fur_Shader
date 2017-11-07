#include "Mesh.h"
#include "Renderer.h"
#include "Macros.h"


Mesh::Mesh()
{
	m_material = std::make_unique<Material>();
}


Mesh::~Mesh()
{
	SAFE_RELEASE(m_vertex_buffer);
}


std::string Mesh::GetName() const
{
	return m_name;
}


Material* Mesh::GetMaterial() const
{
	return m_material.get();
}


void Mesh::CreateMesh(Vertex3D _verticies[], Renderer& _renderer)
{
	// Create Vertex Buffer (passed into GPU VRAM)
	auto vertex_buffer_desc = CD3D11_BUFFER_DESC(sizeof(_verticies), D3D11_BIND_VERTEX_BUFFER);
	D3D11_SUBRESOURCE_DATA vertex_data = { nullptr };
	vertex_data.pSysMem = _verticies;

	auto result = _renderer.GetDevice()->CreateBuffer(&vertex_buffer_desc, &vertex_data, &m_vertex_buffer);

	if (result == S_OK)
		return;

	//if error
	MessageBox(nullptr, "[Mesh](CreateMesh) Failed to create vertex buffer", "Error", MB_OK);
	exit(0);
}


void Mesh::Draw(Renderer& _renderer)
{
	auto device_context = _renderer.GetDeviceContext();

	//TODO: update material shader world matrix based on camera

	// Bind shaders
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	device_context->IASetInputLayout(m_material->GetInputLayout());
	device_context->VSSetShader(m_material->GetVertexShader(), nullptr, 0);
	device_context->PSSetShader(m_material->GetPixelShader(), nullptr, 0);

	// Bind vertex buffer
	UINT stride = sizeof(Vertex3D);// How far the VB must move to access the next vertex
	UINT offset = 0;// Offset between each structure (used to skip info)

	device_context->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);
	device_context->Draw(3, 0);// Draw call (3 is the vertex count)
}