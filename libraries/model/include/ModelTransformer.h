////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Project:  Embedded Machine Learning Library (EMLL)
//  File:     ModelTransformer.h (model)
//  Authors:  Chuck Jacobs
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ModelGraph.h"
#include "Port.h"
#include "InputPort.h"
#include "OutputPort.h"
#include "OutputPortElements.h"
#include "Node.h"
#include "OutputPort.h"

// utilities
#include "Exception.h"

// stl
#include <memory>
#include <vector>
#include <unordered_map>
#include <exception>

/// <summary> model namespace </summary>
namespace model
{
    template <typename ValueType>
    class InputNode;

    class TransformContext
    {
    };

    // TODO: template transformer on the context type
    class ModelTransformer
    {
    public:
        /// <summary> Returns a copy of the input model, by calling Copy() on each of the model's nodes </summary>
        Model CopyModel(const Model& model, const TransformContext& context);

        /// <summary> Returns a refined version of the input model, by calling Refine() on each of the model's nodes </summary>
        Model RefineModel(const Model& model, const TransformContext& context);

        /// <summary> Returns the  OutputPort from new new model corresponding to the given port on the input model </summary>
        /// <remarks> Only available after calling CopyModel or RefineModel </remarks>
        template <typename ValueType>
        const OutputPort<ValueType>* GetCorrespondingOutputPort(const OutputPort<ValueType>& port);

        /// <summary> Returns the input node from new new model corresponding to the given input node on the input model </summary>
        /// <remarks> Only available after calling CopyModel or RefineModel </remarks>
        template <typename ValueType>
        InputNode<ValueType>* GetCorrespondingInputNode(const InputNode<ValueType>* node);

        /// <summary> Returns the (untyped) Port from new new model corresponding to the given port on the input model </summary>
        /// <remarks> Only available after calling CopyModel or RefineModel </remarks>
        const Port* GetCorrespondingPort(const Port& port);

        ///
        /// Functions used by node implementors
        ///

        /// <summary> Transforms a set of output port references from the input model space to the output model space. Called by node implementors. </summary>
        ///
        /// <param name="elements"> The elements in the input model graph to transform to the output model space. </param>
        /// <returns> An OutputPortElements object representing the transformed elements in the space of the new model. </returns>
        template <typename ValueType>
        OutputPortElements<ValueType> TransformOutputPortElements(const OutputPortElements<ValueType>& elements);

        /// <summary> Creates a new node in the transformed model graph. Called by node implementors. </summary>
        ///
        /// <typeparam name="Args"> The arguments to the constructor of NodeType. </typeparam>
        template <typename NodeType, typename... Args>
        NodeType* AddNode(Args&&... args);

        /// <summary> Sets up a port-port mapping. Called by node implementors </summary>
        ///
        /// <param name="oldPort"> The port in the old model to map to the new model. </param>
        /// <param name="newPort"> The port in the new model to be mapped from the old model. </param>
        template <typename ValueType>
        void MapOutputPort(const OutputPort<ValueType>& oldPort, const OutputPort<ValueType>& newPort);

        /// <summary> Get the context used by the transformer. Called by node implementors </summary>
        ///
        /// <returns> The context in use by the transformer. </returns>
        TransformContext& GetContext() { return _context; }

    private:
        friend class Node;

        /// <summary> Sets up a port-port mapping. Called by node implementors </summary>
        void MapPort(const Port& oldPort, const Port& newPort);

        Model _model;
        TransformContext _context;
        std::unordered_map<const Port*, Port*> _portToPortMap;
    };
}

#include "../tcc/ModelTransformer.tcc"
