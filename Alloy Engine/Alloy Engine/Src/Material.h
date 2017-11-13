#pragma once
#include <d3d11.h>
#include <string>
#include "Matrix.h"
#include "Vector3.h"


class Renderer;


//class should be inherited from
class Material
{
public:
	Material();
	virtual ~Material();

	virtual void CreateShaders(std::string _vertex_shader, std::string _pixel_shader, Renderer& _renderer);
	virtual void UpdateBuffers(Renderer& _renderer);

	ID3D11VertexShader* GetVertexShader() const;
	ID3D11PixelShader* GetPixelShader() const;
	ID3D11InputLayout* GetInputLayout() const;

	void SetTransformMatrix(Matrix _transform);
	void SetView(Matrix _view);
	void SetProjection(Matrix _projection);
	void SetCameraPosition(Vector3 _pos);

private:
	ID3D11VertexShader* m_vertex_shader = nullptr;
	ID3D11PixelShader* m_pixel_shader = nullptr;
	ID3D11InputLayout* m_input_layout = nullptr;

	virtual void CreateBuffers(Renderer& _renderer);

	//vertex buffers
	ID3D11Buffer* m_vs_per_object_buffer = nullptr;
	ID3D11Buffer* m_vs_per_frame_buffer = nullptr;

	//pixel buffers
	ID3D11Buffer* m_ps_per_frame_buffer = nullptr;

	struct VSPerFrameBuffer
	{
		Matrix view;
		Matrix proj;
	};

	struct VSPerObjectBuffer
	{
		Matrix transform;
	};


	struct PSPerFrameBuffer
	{
		Vector3 camera_position;
		float extra;//buffer size must be a multiple of 16
	};

	//data to be passed to buffer (dirtyable pattern)
	std::pair<bool, VSPerObjectBuffer> m_vs_per_object;
	std::pair<bool, VSPerFrameBuffer> m_vs_per_frame;
	std::pair<bool, PSPerFrameBuffer> m_ps_per_frame;
};
