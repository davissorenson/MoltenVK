/*
 * MVKCmdPipeline.h
 *
 * Copyright (c) 2014-2018 The Brenwill Workshop Ltd. (http://www.brenwill.com)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "MVKCommand.h"
#include "MVKVector.h"
#include <vector>

class MVKCommandBuffer;
class MVKPipeline;
class MVKPipelineLayout;
class MVKDescriptorSet;
class MVKDescriptorUpdateTemplate;


#pragma mark -
#pragma mark MVKCmdPipelineBarrier

/** Represents an abstract Vulkan command to add a pipeline barrier. */
class MVKCmdPipelineBarrier : public MVKCommand {

public:
	void setContent(VkPipelineStageFlags srcStageMask,
					VkPipelineStageFlags dstStageMask,
					VkDependencyFlags dependencyFlags,
					uint32_t memoryBarrierCount,
					const VkMemoryBarrier* pMemoryBarriers,
					uint32_t bufferMemoryBarrierCount,
					const VkBufferMemoryBarrier* pBufferMemoryBarriers,
					uint32_t imageMemoryBarrierCount,
					const VkImageMemoryBarrier* pImageMemoryBarriers);

	void encode(MVKCommandEncoder* cmdEncoder) override;

	MVKCmdPipelineBarrier(MVKCommandTypePool<MVKCmdPipelineBarrier>* pool);

private:
	VkPipelineStageFlags _srcStageMask;
	VkPipelineStageFlags _dstStageMask;
	VkDependencyFlags _dependencyFlags;
	std::vector<VkMemoryBarrier> _memoryBarriers;
	std::vector<VkBufferMemoryBarrier> _bufferMemoryBarriers;
	std::vector<VkImageMemoryBarrier> _imageMemoryBarriers;
};


#pragma mark -
#pragma mark MVKCmdBindPipeline

/** Vulkan command to bind the pipeline state. */
class MVKCmdBindPipeline : public MVKCommand {

public:
	void setContent(VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline);

	void encode(MVKCommandEncoder* cmdEncoder) override;

	MVKCmdBindPipeline(MVKCommandTypePool<MVKCmdBindPipeline>* pool);

private:
	VkPipelineBindPoint _bindPoint;
	MVKPipeline* _pipeline;

};


#pragma mark -
#pragma mark MVKCmdBindDescriptorSets

/** Vulkan command to bind descriptor sets. */
class MVKCmdBindDescriptorSets : public MVKCommand {

public:
	void setContent(VkPipelineBindPoint pipelineBindPoint,
					VkPipelineLayout layout,
					uint32_t firstSet,
					uint32_t setCount,
					const VkDescriptorSet* pDescriptorSets,
					uint32_t dynamicOffsetCount,
					const uint32_t* pDynamicOffsets);

	void encode(MVKCommandEncoder* cmdEncoder) override;

	MVKCmdBindDescriptorSets(MVKCommandTypePool<MVKCmdBindDescriptorSets>* pool);

private:
	VkPipelineBindPoint _pipelineBindPoint;
	MVKPipelineLayout* _pipelineLayout;
	MVKVector<MVKDescriptorSet*> _descriptorSets;
	MVKVector<uint32_t>          _dynamicOffsets;
	uint32_t _firstSet;
};


#pragma mark -
#pragma mark MVKCmdPushConstants

/** Vulkan command to bind push constants. */
class MVKCmdPushConstants : public MVKCommand {

public:
	void setContent(VkPipelineLayout layout,
					VkShaderStageFlags stageFlags,
					uint32_t offset,
					uint32_t size,
					const void* pValues);

	void encode(MVKCommandEncoder* cmdEncoder) override;

	MVKCmdPushConstants(MVKCommandTypePool<MVKCmdPushConstants>* pool);

private:
	MVKPipelineLayout* _pipelineLayout;
	VkShaderStageFlags _stageFlags;
	uint32_t _offset;
	MVKVector<char> _pushConstants;
};


#pragma mark -
#pragma mark MVKCmdPushDescriptorSet

/** Vulkan command to update a descriptor set. */
class MVKCmdPushDescriptorSet : public MVKCommand {

public:
	void setContent(VkPipelineBindPoint pipelineBindPoint,
					VkPipelineLayout layout,
					uint32_t set,
					uint32_t descriptorWriteCount,
					const VkWriteDescriptorSet* pDescriptorWrites);

	void encode(MVKCommandEncoder* cmdEncoder) override;

	MVKCmdPushDescriptorSet(MVKCommandTypePool<MVKCmdPushDescriptorSet>* pool);

	~MVKCmdPushDescriptorSet() override;

private:
	void clearDescriptorWrites();

	VkPipelineBindPoint _pipelineBindPoint;
	MVKPipelineLayout* _pipelineLayout;
	std::vector<VkWriteDescriptorSet> _descriptorWrites;
	uint32_t _set;
};


#pragma mark -
#pragma mark MVKCmdPushDescriptorSetWithTemplate

/** Vulkan command to update a descriptor set from a template. */
class MVKCmdPushDescriptorSetWithTemplate : public MVKCommand {

public:
	void setContent(VkDescriptorUpdateTemplateKHR descUpdateTemplate,
					VkPipelineLayout layout,
					uint32_t set,
					const void* pData);

	void encode(MVKCommandEncoder* cmdEncoder) override;

	MVKCmdPushDescriptorSetWithTemplate(MVKCommandTypePool<MVKCmdPushDescriptorSetWithTemplate>* pool);

	~MVKCmdPushDescriptorSetWithTemplate() override;

private:
	MVKDescriptorUpdateTemplate* _descUpdateTemplate;
	MVKPipelineLayout* _pipelineLayout;
	uint32_t _set;
	void* _pData;
};


#pragma mark -
#pragma mark Command creation functions

/** Adds commands to the specified command buffer that insert the specified pipeline barriers. */
void mvkCmdPipelineBarrier(MVKCommandBuffer* cmdBuff,
						   VkPipelineStageFlags srcStageMask,
						   VkPipelineStageFlags dstStageMask,
						   VkDependencyFlags dependencyFlags,
						   uint32_t memoryBarrierCount,
						   const VkMemoryBarrier* pMemoryBarriers,
						   uint32_t bufferMemoryBarrierCount,
						   const VkBufferMemoryBarrier* pBufferMemoryBarriers,
						   uint32_t imageMemoryBarrierCount,
						   const VkImageMemoryBarrier* pImageMemoryBarriers);

/** Adds a command to the specified command buffer that binds the specified pipeline. */
void mvkCmdBindPipeline(MVKCommandBuffer* cmdBuff,
						VkPipelineBindPoint pipelineBindPoint,
						VkPipeline pipeline);

/** Adds commands to the specified command buffer that insert the specified descriptor sets. */
void mvkCmdBindDescriptorSets(MVKCommandBuffer* cmdBuff,
							  VkPipelineBindPoint pipelineBindPoint,
							  VkPipelineLayout layout,
							  uint32_t firstSet,
							  uint32_t setCount,
							  const VkDescriptorSet* pDescriptorSets,
							  uint32_t dynamicOffsetCount,
							  const uint32_t* pDynamicOffsets);

/** Adds a vertex bind command to the specified command buffer. */
void mvkCmdPushConstants(MVKCommandBuffer* cmdBuff,
						 VkPipelineLayout layout,
						 VkShaderStageFlags stageFlags,
						 uint32_t offset,
						 uint32_t size,
						 const void* pValues);

/** Adds commands to the specified command buffer that update the specified descriptor set. */
void mvkCmdPushDescriptorSet(MVKCommandBuffer* cmdBuff,
							 VkPipelineBindPoint pipelineBindPoint,
							 VkPipelineLayout layout,
							 uint32_t set,
							 uint32_t descriptorWriteCount,
							 const VkWriteDescriptorSet* pDescriptorWrites);

/** Adds commands to the specified command buffer that update the specified descriptor set from the given template. */
void mvkCmdPushDescriptorSetWithTemplate(MVKCommandBuffer* cmdBuff,
										 VkDescriptorUpdateTemplateKHR descUpdateTemplate,
										 VkPipelineLayout layout,
										 uint32_t set,
										 const void* pData);
