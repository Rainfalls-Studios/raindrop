// #include <Raindrop/Graphics/Editor/Camera.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtx/euler_angles.hpp>

// namespace Raindrop::Graphics::Editor{
// 	void Camera::update(){

// 		forward.x = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
// 		forward.y = sin(glm::radians(rotation.x));
// 		forward.z = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
// 		forward = glm::normalize(forward);

// 		right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
// 		up = glm::normalize(glm::cross(right, forward));

// 		view = glm::lookAt(translation, translation + forward, up);

// 		switch (type){
// 			case ORTHOGRAPHIC:{
// 				projection = glm::ortho(width.x, width.y, height.x, height.y, depth.x, depth.y); 
// 				break;
// 			}

// 			case PERSPECTIVE:{
// 				float aspectRatio = glm::abs(width.x - width.y) / glm::abs(height.x - height.y);
// 				projection = glm::perspective(fov, aspectRatio, depth.x, depth.y);
// 				break;
// 			}
// 		}

// 		viewProjection = projection * view;
// 	}
// }