#include "Mesh.h"
#include "Renderer.h"
#include "Macros.h"


Mesh::Mesh(std::string _name)
{
	m_name = _name;
	m_material = new Material();
}


Mesh::~Mesh()
{
	SAFE_RELEASE(m_vertex_buffer);
	SAFE_DELETE(m_material);
}


std::string Mesh::GetName() const
{
	return m_name;
}


Material* Mesh::GetMaterial() const
{
	return m_material;
}


void Mesh::CreateMesh(const std::vector<Vertex3D>& _vertices, const std::vector<unsigned int>& _indicies, Renderer& _renderer)
{
	// Create Vertex Buffer (passed into GPU VRAM)
	auto vertex_buffer_desc = CD3D11_BUFFER_DESC();
	vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertex_buffer_desc.CPUAccessFlags = 0x00;
	vertex_buffer_desc.MiscFlags = 0x00;
	vertex_buffer_desc.ByteWidth = sizeof(Vertex3D) * _vertices.size();
	vertex_buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
	vertex_buffer_desc.StructureByteStride = 0x00;

	D3D11_SUBRESOURCE_DATA vertex_data = { nullptr };
	vertex_data.pSysMem = &_vertices[0];

	auto result = _renderer.GetDevice()->CreateBuffer(&vertex_buffer_desc, &vertex_data, &m_vertex_buffer);

	if (result != S_OK)
		MessageBox(nullptr, "[Mesh](CreateMesh) Failed to create vertex buffer", "Error", MB_OK);

	D3D11_BUFFER_DESC indicie_buffer_desc = {};
	indicie_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indicie_buffer_desc.CPUAccessFlags = 0x00;
	indicie_buffer_desc.MiscFlags = 0x00;
	indicie_buffer_desc.ByteWidth = sizeof(std::uint32_t) * _indicies.size();
	indicie_buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
	indicie_buffer_desc.StructureByteStride = 0x00;

	D3D11_SUBRESOURCE_DATA index_data = {};
	index_data.pSysMem = &_indicies[0];

	result = _renderer.GetDevice()->CreateBuffer(&indicie_buffer_desc, &index_data, &m_index_buffer);

	m_indicie_count = _indicies.size();

	if (result != S_OK)
		MessageBox(nullptr, "[Mesh](CreateMesh) Failed to create index buffer", "Error", MB_OK);
}


void Mesh::Draw(Renderer& _renderer)
{
	auto device_context = _renderer.GetDeviceContext();

	//update material
	m_material->SetProjection(_renderer.GetRenderData()->camera_projection_matrix);
	m_material->SetView(_renderer.GetRenderData()->camera_view_matrix);
	m_material->SetTransformMatrix(m_transform.GetTransformMatrix());
	m_material->UpdateBuffers(_renderer);

	// Bind shaders
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	device_context->IASetInputLayout(m_material->GetInputLayout());
	device_context->VSSetShader(m_material->GetVertexShader(), nullptr, 0);
	device_context->PSSetShader(m_material->GetPixelShader(), nullptr, 0);

	// Bind vertex buffer
	UINT stride = sizeof(Vertex3D);// How far the VB must move to access the next vertex
	UINT offset = 0;// Offset between each structure (used to skip info)

	device_context->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);
	device_context->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, 0);
	device_context->Draw(m_indicie_count, 0);// Draw call
}