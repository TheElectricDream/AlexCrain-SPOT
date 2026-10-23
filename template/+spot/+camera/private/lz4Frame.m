function data = lz4Frame(packed)
    %LZ4FRAME Decompress LZ4 frame data, with the Java libraries that ship with MATLAB.
    %   data = lz4Frame(packed) for spot.camera's converters (AEDAT4 packets, MCAP chunks).
    %
    %   Inputs
    %     packed  LZ4 frame format bytes, uint8 vector
    %
    %   Outputs
    %     data    the decompressed bytes, uint8 column
    %
    %   Behavior
    %     1. Apache Commons Compress's frame reader, which takes blocks that depend on the
    %        ones before them (dv-processing writes them so; MATLAB's lz4-java can't).
    %
    %   See also spot.camera.dvxplorerToMkv, spot.camera.zedToMkv.

    %% Decompress
    stream = org.apache.commons.compress.compressors.lz4.FramedLZ4CompressorInputStream( ...
        java.io.ByteArrayInputStream(packed));
    closeStream = onCleanup(@() stream.close());
    data = typecast(org.apache.commons.io.IOUtils.toByteArray(stream), 'uint8');
    data = data(:);
end
