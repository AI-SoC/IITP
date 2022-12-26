def separable_conv2d(input, w_depth, w_pointwise):
    """Depthwise separable convolution.

    Performs 2d depthwise convolution with w_depth, and then applies a pointwise
    1x1 convolution with w_pointwise on the result.

    Uses SAME padding with 0s, a stride of 1 and no dilation. A single output
    channel is used per input channel (channel_multiplier=1) in w_depth.

    input: input array with shape (height, width, in_depth)
    w_depth: depthwise filter array with shape (fd, fd, in_depth)
    w_pointwise: pointwise filter array with shape (in_depth, out_depth)

    Returns a result with shape (height, width, out_depth).
    """
    # First run the depthwise convolution. Its result has the same shape as
    # input.
    depthwise_result = depthwise_conv2d(input, w_depth)

    height, width, in_depth = depthwise_result.shape
    assert in_depth == w_pointwise.shape[0]
    out_depth = w_pointwise.shape[1]
    output = np.zeros((height, width, out_depth))

    for out_c in range(out_depth):
        for i in range(height):
            for j in range(width):
                for c in range(in_depth):
                    w_element = w_pointwise[c, out_c]
                    output[i, j, out_c] += depthwise_result[i, j, c] * w_element
    return output
