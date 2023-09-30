
#version 430

// NOTE: don't write complex macro expressions
// NOTE: don't use shared variables as function argument

#define BLOCK_SIZE	16
#define MAX_DEGREE	3
#define MAX_ORDER	4
#define MAX_CVS		10
#define MAX_KNOTS	14
#define MAX_SPANS	13

struct Vertex
{
	vec4 Position;
	vec4 Normal;
};

struct Tile
{
	uint Indices[6];
};

layout(std140, binding = 0) buffer VertexBuffer {
	Vertex data[];
} surfacevertices;

layout(std430, binding = 1) buffer IndexBuffer {
	Tile data[];
} surfaceindices;

uniform vec4 controlPoints[100];
uniform float knotsU[MAX_KNOTS];
uniform float knotsV[MAX_KNOTS];
uniform float weightsU[MAX_CVS];
uniform float weightsV[MAX_CVS];

uniform int numVerticesU;
uniform int numVerticesV;
uniform int numControlPointsU;
uniform int numControlPointsV;
uniform int degreeU;
uniform int degreeV;

// vec4 <=> max degree 3 curves
shared vec4 CoeffsU[52];	// MAX_ORDER * MAX_SPANS
shared vec4 CoeffsV[52];	// MAX_ORDER * MAX_SPANS
shared float Blossom[64];	// MAX_ORDER * MAX_ORDER * MAX_ORDER

float CalculateCoeff(int i, int degree, int e, int span, float knots[MAX_KNOTS])
{
	float	kin1, ki1, kin, ki;
	float	a, b;
	int		order = degree + 1;
	int		k, n, l;
	int		index1, index2, index3;

	if (e > degree)
		return 0.0;

	for (k = 0; k <= e; ++k) {
		for (n = k; n < order; ++n) {
			for (l = 0; l < order - n; ++l) {
				kin1 = knots[i + l + n + 1];
				ki1 = knots[i + l + 1];
				kin = knots[i + l + n];
				ki = knots[i + l];

				a = b = 0;

				index1 = (k * MAX_ORDER + n) * MAX_ORDER + l;
				index2 = (k * MAX_ORDER + n - 1) * MAX_ORDER + l;
				index3 = ((k - 1) * MAX_ORDER + n - 1) * MAX_ORDER + l;

				if (n == 0) {
					// C_i,0,0
					Blossom[index1] = ((i + l == span) ? 1.0 : 0.0);
				}
				else if (k == 0) {
					// C_i,n,0
					if (kin1 != ki1)
						a = kin1 / (kin1 - ki1);

					if (kin != ki)
						b = ki / (kin - ki);

					Blossom[index1] = Blossom[index2 + 1] * a - Blossom[index2] * b;
				}
				else if (k == n) {
					// C_i,n,n
					if (kin != ki)
						a = 1.0f / (kin - ki);

					if (kin1 != ki1)
						b = 1.0f / (kin1 - ki1);

					Blossom[index1] = Blossom[index3] * a - Blossom[index3 + 1] * b;
				}
				else {
					// C_i,n,k
					if (kin != ki)
						a = (Blossom[index3] - ki * Blossom[index2]) / (kin - ki);

					if (kin1 != ki1)
						b = (Blossom[index3 + 1] - kin1 * Blossom[index2 + 1]) / (kin1 - ki1);

					Blossom[index1] = a - b;
				}
			}
		}
	}

	return Blossom[(e * MAX_ORDER + degree) * MAX_ORDER];
}

layout(local_size_x = BLOCK_SIZE, local_size_y = BLOCK_SIZE) in;
void main()
{
	vec4 cf;

	int numknotsU = numControlPointsU + degreeU + 1;
	int numknotsV = numControlPointsV + degreeV + 1;
	int numsegmentsU = numVerticesU - 1;
	int numsegmentsV = numVerticesV - 1;
	int lastspanU = numControlPointsU - 1;
	int lastspanV = numControlPointsV - 1;
	int orderU = degreeU + 1;
	int orderV = degreeV + 1;
	int spanU, spanV, cpU, cpV;
	int index1, index2;

	// STEP 1: calculate coefficients
	if (gl_LocalInvocationIndex == 0) {
		// NOTE: don't refactor this into a function, it won't work!!!
		for (int spanU = 0; spanU < numknotsU - 1; ++spanU) {
			for (int k = 0; k <= min(degreeU, spanU); ++k) {
				index1 = spanU * MAX_ORDER + k;

				if (spanU - k < numControlPointsU) {
					cpU = spanU - k;

					cf.x = CalculateCoeff(cpU, degreeU, 0, spanU, knotsU); // 1
					cf.y = CalculateCoeff(cpU, degreeU, 1, spanU, knotsU); // u
					cf.z = CalculateCoeff(cpU, degreeU, 2, spanU, knotsU); // u2
					cf.w = CalculateCoeff(cpU, degreeU, 3, spanU, knotsU); // u3

					CoeffsU[index1] = cf;
				}
				else {
					CoeffsU[index1] = vec4(0.0);
				}
			}
		}

		// NOTE: don't refactor this into a function, it won't work!!!
		for (int spanV = 0; spanV < numknotsV - 1; ++spanV) {
			for (int k = 0; k <= min(degreeV, spanV); ++k) {
				index2 = spanV * MAX_ORDER + k;

				if (spanV - k < numControlPointsV) {
					cpV = spanV - k;

					cf.x = CalculateCoeff(cpV, degreeV, 0, spanV, knotsV); // 1
					cf.y = CalculateCoeff(cpV, degreeV, 1, spanV, knotsV); // v
					cf.z = CalculateCoeff(cpV, degreeV, 2, spanV, knotsV); // v2
					cf.w = CalculateCoeff(cpV, degreeV, 3, spanV, knotsV); // v3

					CoeffsV[index2] = cf;
				}
				else {
					CoeffsV[index2] = vec4(0.0);
				}
			}
		}
	}

	barrier();

	// STEP 2: fill vertex buffer
	vec4 pos;
	vec4 polyU, polyV;
	vec4 dUpoly, dVpoly;
	vec3 tangent, bitangent;

	ivec2 loc = ivec2(gl_LocalInvocationID.xy);
	int elemsperitemU = max(1, (numVerticesU + (BLOCK_SIZE - 1)) / BLOCK_SIZE);
	int elemsperitemV = max(1, (numVerticesV + (BLOCK_SIZE - 1)) / BLOCK_SIZE);
	int startU = loc.x * elemsperitemU;
	int startV = loc.y * elemsperitemV;
	int endU = min(startU + elemsperitemU, numVerticesU);
	int endV = min(startV + elemsperitemV, numVerticesV);

	float firstU = knotsU[degreeU];
	float firstV = knotsV[degreeV];
	float lastU = knotsU[numControlPointsU];
	float lastV = knotsV[numControlPointsV];
	float u, v, nom, denom, weight;
	float c, cdU, cdV;
	vec3 dUf, dVf;
	float dUg, dVg;

	for (int i = startU; i < endU; ++i) {
		u = float(i) / float(numsegmentsU);
		u = mix(firstU, lastU, u);

		polyU = vec4(1.0, u, u * u, u * u * u);
		dUpoly = vec4(0.0, 1.0, 2.0 * u, 3.0 * u * u);

		for (spanU = 0; spanU < lastspanU; ++spanU) {
			if ((knotsU[spanU] <= u && u < knotsU[spanU + 1]))
				break;
		}

		for (int j = startV; j < endV; ++j) {
			v = float(j) / float(numsegmentsV);
			v = mix(firstV, lastV, v);

			polyV = vec4(1.0, v, v * v, v * v * v);
			dVpoly = vec4(0.0, 1.0, 2.0 * v, 3.0 * v * v);

			for (spanV = 0; spanV < lastspanV; ++spanV) {
				if ((knotsV[spanV] <= v && v < knotsV[spanV + 1]))
					break;
			}

			// sum contributions
			pos = vec4(0.0);
			dUf = vec3(0.0);
			dVf = vec3(0.0);

			denom = 0.0;
			dUg = 0.0;
			dVg = 0.0;

			for (int k = 0; k < orderU; ++k) {
				for (int l = 0; l < orderV; ++l) {
					index1 = spanU * MAX_ORDER + k;
					index2 = spanV * MAX_ORDER + l;

					cpU = (spanU - k) % numControlPointsU;
					cpV = (spanV - l) % numControlPointsV;

					cf = controlPoints[cpU * numControlPointsV + cpV];
					weight = weightsU[cpU] * weightsV[cpV];

					c = dot(CoeffsU[index1], polyU) * dot(CoeffsV[index2], polyV) * weight;
					cdU = dot(CoeffsU[index1], dUpoly) * dot(CoeffsV[index2], polyV) * weight;
					cdV = dot(CoeffsU[index1], polyU) * dot(CoeffsV[index2], dVpoly) * weight;

					pos += cf * c;	// f
					denom += c;		// g

					dUg += cdU;
					dVg += cdV;

					dUf += cf.xyz * cdU;
					dVf += cf.xyz * cdV;
				}
			}

			tangent = (dUf * denom - dUg * pos.xyz) / (denom * denom);
			bitangent = (dVf * denom - dVg * pos.xyz) / (denom * denom);
			pos /= denom;

			pos.w = 1.0;
			index1 = i * numVerticesV + j;

			surfacevertices.data[index1].Position = pos;
			surfacevertices.data[index1].Normal = vec4(normalize(cross(bitangent, tangent)), 0.0);
		}
	}

	// STEP 3: fill index buffer
	endU = min(endU, numsegmentsU);
	endV = min(endV, numsegmentsV);

	for (int i = startU; i < endU; ++i) {
		for (int j = startV; j < endV; ++j) {
			index1 = i * numsegmentsV + j;

			surfaceindices.data[index1].Indices[0] = i * numVerticesV + j;
			surfaceindices.data[index1].Indices[1] = i * numVerticesV + j + 1;
			surfaceindices.data[index1].Indices[2] = (i + 1) * numVerticesV + j + 1;

			surfaceindices.data[index1].Indices[3] = i * numVerticesV + j;
			surfaceindices.data[index1].Indices[4] = (i + 1) * numVerticesV + j + 1;
			surfaceindices.data[index1].Indices[5] = (i + 1) * numVerticesV + j;
		}
	}
}
