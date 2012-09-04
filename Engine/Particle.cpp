#include "Particle.h"

Particle::Particle(Vector3 pos, bool loop)
{
	Position = pos;
	timer = 0;
	texX = 0; texY = 0;
	isPlaying = true;
	isLoop = loop;
	isFinished = false;
}

Particle::~Particle(void)
{
}

void Particle::Draw()
{
	if (!isFinished && isPlaying)
	{
		Matrix mpos, mrotx, mroty, mrotz, mrot, mview, mproj, mscale;
		mscale.SetScale(Scale);
		mpos.SetTranslation(Position);
		mrotx.SetRotationX(Rotation.x);
		mroty.SetRotationY(Rotation.y);
		mrotz.SetRotationZ(Rotation.z);
		mrot = mroty * mrotx * mrotz;
		mvpMatrix = mscale * mrot * mpos * Camera::GetInstance()->getViewMatrix() * Camera::GetInstance()->getProjectionMatrix();

		GL2D::DrawBillboard(23, Position.x, Position.y + 1.0f, Position.z, texX, texY, 110, 128);
		timer+=0.25f;
		if (timer >= 1)
		{
			timer = 0;
			texX++;
			if (texX >= 8) 
			{
				texX = 0;
				texY++;
			}
			if (texY >= 7) 
			{
				if (texX > 3)
				{
					if (isLoop)
					{
						texY = 0;
						texX = 0;
					}
					else
					{
						isFinished = true;
						isPlaying = false;
					}
				}
			}
		}
	}
}
